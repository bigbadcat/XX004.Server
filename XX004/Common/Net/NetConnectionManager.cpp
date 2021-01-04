/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnectionManager.cpp
* Summary: 网络通信模块服务端连接管理器。
*
* Author: XuXiang
* Date: 2018-07-30 23:07
*******************************************************/

#include "NetConnectionManager.h"
#include "NetServer.h"
#include "NetListener.h"
#include "NetConnection.h"
#include "../Protocol/NetMsgID.h"
#include <assert.h>
#include <WS2tcpip.h>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetAwakeBridge::NetAwakeBridge() : m_Send(SOCKET_ERROR)
		{

		}

		NetAwakeBridge::~NetAwakeBridge()
		{
			SAFE_CLOSE_SOCKET(m_Send);
		}

		void NetAwakeBridge::Init(int port)
		{
			socket_t s = ::socket(AF_INET, SOCK_STREAM, 0);
			if (s == SOCKET_ERROR)
			{
				cout << "create socket err:" << GET_LAST_ERROR() << endl;
				return;
			}

			SOCKADDR_IN addrSrv;
			int pton = inet_pton(AF_INET, "127.0.0.1", &addrSrv.sin_addr.S_un.S_addr);
			addrSrv.sin_family = AF_INET;
			addrSrv.sin_port = htons((u_short)port);

			//非阻塞
			if (!socket_set_nonblocking(s, true))
			{
				cout << "set socket noblocking err:" << GET_LAST_ERROR() << endl;
				SAFE_CLOSE_SOCKET(s);
				return;
			}
			::connect(s, (sockaddr *)&addrSrv, sizeof (sockaddr));
			m_Send = s;
		}

		void NetAwakeBridge::Notify()
		{
			if (!CheckSend())
			{
				return;
			}

			//发送一个唤醒协议
			Byte sendbuff[1024];
			int sendsize = 0;
			NetPackageHeader sendhead;
			sendhead.SetSign();
			sendhead.Command = MsgID::INTERNAL_AWAKE;
			sendhead.GUID = 0;
			sendhead.BodySize = 0;
			sendsize = sendhead.Pack(sendbuff, sendsize);

			int ret = ::send(m_Send, (char*)sendbuff, sendsize, 0);
			if (ret == SOCKET_ERROR)
			{
				cout << "send socket err:" << GET_LAST_ERROR() << endl;
				SAFE_CLOSE_SOCKET(m_Send)
			}
		}

		bool NetAwakeBridge::CheckSend()
		{
			if (m_Send == SOCKET_ERROR)
			{
				return false;
			}

			bool ok = false;
			fd_set readfds;
			fd_set writefds;
			fd_set exceptfds;
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
			FD_SET(m_Send, &writefds);
			FD_SET(m_Send, &exceptfds);
			timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			int ret = ::select(0, &readfds, &writefds, &exceptfds, &timeout);		//windows下nfds参数无用，可传入0
			if (ret > 0)
			{
				//先判断是否有异常
				if (FD_ISSET(m_Send, &exceptfds) != 0)
				{
					cout << "NetAwakeBridge error:" << GET_LAST_ERROR() << endl;
					SAFE_CLOSE_SOCKET(m_Send);
				}
				//可以写入了则说明已经准备好了
				if (FD_ISSET(m_Send, &writefds) != 0)
				{
					ok = true;
				}
			}
			else if (ret == SOCKET_ERROR)
			{
				cout << "NetAwakeBridge error:" << GET_LAST_ERROR() << endl;
				SAFE_CLOSE_SOCKET(m_Send)
			}
			return ok;
		}


		NetConnectionManager::NetConnectionManager() : m_pServer(NULL), m_pListener(NULL)
		{
			m_pListener = new NetListener();
			m_pListener->SetManager(this);
		}

		NetConnectionManager::~NetConnectionManager()
		{
			Release();
			SAFE_DELETE(m_pListener)
		}

		void NetConnectionManager::Init(int port)
		{
			m_pListener->Start(port);
			m_AwakeBridge.Init(port);
		}

		void NetConnectionManager::Release()
		{
			m_pListener->Stop();
			m_RemoteKeyToConnection.clear();
			SAFE_DELETE_MAP(m_Connections);
		}

		void NetConnectionManager::SelectSocket(int msec)
		{
			//准备文件集合
			fd_set readfds;
			fd_set writefds;
			fd_set exceptfds;
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
			if (m_pListener->GetSocket() != SOCKET_ERROR)
			{
				socket_t s = m_pListener->GetSocket();
				FD_SET(s, &readfds);
				FD_SET(s, &exceptfds);
			}
			for (NetConnectionMap::iterator itr = m_Connections.begin(); itr != m_Connections.end(); ++itr)
			{
				NetConnection *con = itr->second;
				socket_t s = con->GetSocket();
				if (con->IsNeedRead()) { FD_SET(s, &readfds); }
				if (con->IsNeedWrite()) { FD_SET(s, &writefds); }
				FD_SET(s, &exceptfds);
			}

			//处理
			timeval timeout;
			timeout.tv_sec = msec / 1000;				//秒
			timeout.tv_usec = (msec % 1000) * 1000;		//毫秒=1000微秒
			int ret = ::select(0, &readfds, &writefds, &exceptfds, &timeout);		//windows下nfds参数无用，可传入0
			if (ret > 0)
			{
				vector<socket_t> needremove;
				if (m_pListener->GetSocket() != SOCKET_ERROR)
				{
					socket_t s = m_pListener->GetSocket();
					if (FD_ISSET(s, &exceptfds) != 0)
					{
						needremove.push_back(s);
					}
					else if (FD_ISSET(s, &readfds) != 0 && OnSocketRead(s) != 0)
					{
						needremove.push_back(s);
					}
				}
				for (NetConnectionMap::iterator itr = m_Connections.begin(); itr != m_Connections.end(); ++itr)
				{
					//先判断是否有异常，再判断读写
					socket_t s = itr->second->GetSocket();
					if (FD_ISSET(s, &exceptfds) != 0)
					{
						needremove.push_back(s);
						continue;
					}
					if (FD_ISSET(s, &readfds) != 0 && OnSocketRead(s) != 0)
					{
						needremove.push_back(s);
						continue;
					}
					if (FD_ISSET(s, &writefds) != 0 && OnSocketWrite(s) != 0)
					{
						needremove.push_back(s);
						continue;
					}
				}

				//移除错误的SOKECT
				for (vector<socket_t>::iterator itr = needremove.begin(); itr != needremove.end(); ++itr)
				{
					OnSocketClose(*itr);
				}
			}
			else if (ret == SOCKET_ERROR)
			{
				cout << "select socket err:" << GET_LAST_ERROR() << endl;
			}
		}

		void NetConnectionManager::AddConnection(socket_t s)
		{
			assert(m_Connections.find(s) == m_Connections.end());
			if (m_Connections.size() >= 60)		//Win下默认是能处理64个socket，一个Listener，三个预留
			{
				::printf_s("AddConnection failed\n");
				SAFE_CLOSE_SOCKET(s);
				return;
			}

			NetConnection *pcon = new NetConnection();
			pcon->SetSocket(s);
			m_Connections.insert(NetConnectionMap::value_type(s, pcon));

			//通知
			if (m_pServer != NULL)
			{
				m_pServer->OnConnect(pcon);
			}
		}

		void NetConnectionManager::RemoveConnection(NetConnection* con)
		{
			assert(con != NULL);
			socket_t s = con->GetSocket();
			OnSocketClose(con->GetSocket());
		}

		NetConnection* NetConnectionManager::GetConnection(const RemoteKey &key)const
		{
			assert(key.first != RemoteType::RT_UNKNOW);
			RemoteKeyToConnectionMap::const_iterator itr = m_RemoteKeyToConnection.find(key);
			return itr == m_RemoteKeyToConnection.cend() ? NULL : itr->second;
		}

		void NetConnectionManager::SetRemote(UInt64 uid, const RemoteKey &key)
		{
			NetConnection *con = GetConnection(uid);
			assert(con != NULL);
			assert(con->GetRomoteType() == RemoteType::RT_UNKNOW || con->GetRomoteType() == key.first);			//必须是小白连接或同类型
			assert(key.first != RemoteType::RT_UNKNOW);		//参数合法
			con->SetRemote(key);

			//还没有设定RoleID的客户端连接先不建立key映射
			if (key.first != RemoteType::RT_CLIENT || key.second != 0)
			{
				m_RemoteKeyToConnection[key] = con;
			}
		}

		void NetConnectionManager::OnRecvPackage(NetConnection *con)
		{
			assert(con != NULL && m_pServer != NULL);
			m_pServer->OnRecvData(con);
		}

		NetConnection* NetConnectionManager::GetConnectionFromSocket(socket_t s)const
		{
			NetConnectionMap::const_iterator itr = m_Connections.find(s);
			return itr != m_Connections.end() ? itr->second : NULL;
		}

		int NetConnectionManager::OnSocketRead(socket_t s)
		{
			if (s == m_pListener->GetSocket())
			{
				return m_pListener->OnSocketRead();
			}

			static Byte buff[1024];
			NetConnection *pcon = GetConnectionFromSocket(s);
			if (pcon == NULL)
			{
				return 0;
			}

			int len = ::recv(pcon->GetSocket(), (char*)buff, 1024, 0);
			if (len > 0)
			{
				if (!pcon->AddRecvData(buff, len))
				{
					return 3;
				}

				do
				{
					int ret = pcon->CheckRecvPackage();
					if (ret == 0)
					{
						break;
					}
					else if (ret == 1)
					{
						OnRecvPackage(pcon);
					}
					else
					{
						return 4;
					}
				} while (true);
			}
			else if (len == 0)
			{
				//客户端关闭了连接
				return 1;
			}
			else
			{
				cout << "recv socket err:" << GET_LAST_ERROR() << endl;
				return 2;
			}

			return 0;
		}

		int NetConnectionManager::OnSocketWrite(socket_t s)
		{
			NetConnection *pcon = GetConnectionFromSocket(s);
			if (pcon == NULL)
			{
				return 0;
			}
			return pcon->DoSend();
		}

		void NetConnectionManager::OnSocketClose(socket_t s)
		{
			if (s == m_pListener->GetSocket())
			{
				m_pListener->OnSocketClose();
				return;
			}

			//连接断开
			NetConnection *pcon = GetConnectionFromSocket(s);
			if (pcon == NULL)
			{
				return;
			}

			//先通知，再销毁
			if (m_pServer != NULL)
			{
				m_pServer->OnDisconnect(pcon);
			}

			m_Connections.erase(s);
			m_RemoteKeyToConnection.erase(pcon->GetRemote());
			pcon->SetSocket(SOCKET_ERROR);
			SAFE_DELETE(pcon);
			SAFE_CLOSE_SOCKET(s);
		}
	}
}