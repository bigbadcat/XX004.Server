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
#include <assert.h>
using namespace std;

namespace XX004
{
	namespace Net
	{
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
		}

		void NetConnectionManager::Release()
		{
			m_pListener->Stop();
			m_UIDToConnection.clear();
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
				SOCKET s = m_pListener->GetSocket();
				FD_SET(s, &readfds);
				FD_SET(s, &exceptfds);
			}
			for (NetConnectionMap::iterator itr = m_Connections.begin(); itr != m_Connections.end(); ++itr)
			{
				NetConnection *con = itr->second;
				SOCKET s = con->GetSocket();
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
				vector<SOCKET> needremove;
				if (m_pListener->GetSocket() != SOCKET_ERROR)
				{
					SOCKET s = m_pListener->GetSocket();
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
					SOCKET s = itr->second->GetSocket();
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
				for (vector<SOCKET>::iterator itr = needremove.begin(); itr != needremove.end(); ++itr)
				{
					OnSocketClose(*itr);
				}
			}
			else if (ret == SOCKET_ERROR)
			{
				cout << "select socket err:" << WSAGetLastError() << endl;
			}
		}

		void NetConnectionManager::AddConnection(SOCKET s)
		{
			assert(m_Connections.find(s) == m_Connections.end());
			if (m_Connections.size() >= 60)		//Win下默认是能处理64个SOCKET，一个Listener，三个预留
			{
				::printf_s("AddConnection failed\n");
				SAFE_CLOSE_SOCKET(s);
				return;
			}

			static UInt64 cur_uid = 0;
			NetConnection *pcon = new NetConnection();
			pcon->SetUniqueID(++cur_uid);
			pcon->SetSocket(s);
			m_Connections.insert(NetConnectionMap::value_type(s, pcon));
			m_UIDToConnection.insert(UIDToConnectionMap::value_type(pcon->GetUniqueID(), pcon));

			//通知
			if (m_pServer != NULL)
			{
				m_pServer->OnConnect(pcon);
			}
		}

		void NetConnectionManager::RemoveConnection(NetConnection* con)
		{
			assert(con != NULL);
			SOCKET s = con->GetSocket();			
			OnSocketClose(con->GetSocket());
		}

		NetConnection* NetConnectionManager::GetConnection(UInt64 uid)const
		{
			NetConnectionMap::const_iterator itr = m_UIDToConnection.find(uid);
			return itr == m_UIDToConnection.cend() ? NULL : itr->second;
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

		NetConnection* NetConnectionManager::GetConnectionFromSocket(SOCKET s)const
		{
			NetConnectionMap::const_iterator itr = m_Connections.find(s);
			return itr != m_Connections.end() ? itr->second : NULL;
		}

		int NetConnectionManager::OnSocketRead(SOCKET s)
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
				cout << "recv socket err:" << WSAGetLastError() << endl;
				return 2;
			}

			return 0;
		}

		int NetConnectionManager::OnSocketWrite(SOCKET s)
		{
			NetConnection *pcon = GetConnectionFromSocket(s);
			if (pcon == NULL)
			{
				return 0;
			}
			return pcon->DoSend();
		}

		void NetConnectionManager::OnSocketClose(SOCKET s)
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
			m_UIDToConnection.erase(pcon->GetUniqueID());
			m_RemoteKeyToConnection.erase(pcon->GetRemote());
			pcon->SetSocket(SOCKET_ERROR);
			SAFE_DELETE(pcon);
			SAFE_CLOSE_SOCKET(s);
		}
	}
}