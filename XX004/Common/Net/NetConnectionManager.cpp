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
//#include "NetConnectionSet.h"
#include <assert.h>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetConnectionManager::NetConnectionManager() : m_pServer(NULL)
		{

		}

		NetConnectionManager::~NetConnectionManager()
		{
			Release();
		}

		void NetConnectionManager::Init()
		{

		}

		void NetConnectionManager::Release()
		{
			m_UIDToConnection.clear();
			m_RemoteKeyToConnection.clear();
			SAFE_DELETE_MAP(m_Connections);
		}

		void NetConnectionManager::SelectSocket()
		{
			if (m_Connections.size() <= 0)
			{
				return;
			}

			fd_set readfds;
			fd_set writefds;
			fd_set exceptfds;
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
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
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			int ret = ::select(0, &readfds, &writefds, &exceptfds, &timeout);		//windows下nfds参数无用，可传入0
			if (ret > 0)
			{
				vector<SOCKET> needremove;
				for (NetConnectionMap::iterator itr = m_Connections.begin(); itr != m_Connections.end(); ++itr)
				{
					SOCKET s = itr->second->GetSocket();

					//先判断是否有异常
					if (FD_ISSET(s, &exceptfds) != 0)
					{
						needremove.push_back(s);
						continue;
					}
					if (FD_ISSET(s, &readfds) != 0)
					{
						if (OnSocketRead(s) != 0)
						{
							needremove.push_back(s);
							continue;
						}
					}
					if (FD_ISSET(s, &writefds) != 0)
					{
						if (OnSocketWrite(s) != 0)
						{
							needremove.push_back(s);
							continue;
						}
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

		NetConnection* NetConnectionManager::AddConnection(SOCKET s)
		{
			assert(m_Connections.find(s) == m_Connections.end());
			if (m_Connections.size() >= 60)		//Win下默认是能处理64个SOCKET，一个Listener，三个预留
			{
				return NULL;
			}

			static UInt64 cur_uid = 0;
			NetConnection *pcon = new NetConnection();
			pcon->SetUniqueID(++cur_uid);
			pcon->SetSocket(s);
			m_Connections.insert(NetConnectionMap::value_type(s, pcon));
			m_UIDToConnection.insert(UIDToConnectionMap::value_type(pcon->GetUniqueID(), pcon));
			return pcon;
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