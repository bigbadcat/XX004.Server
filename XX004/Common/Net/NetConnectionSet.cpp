/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnectionSet.cpp
* Summary: 网络通信模块服务端连接集合。
*
* Author: XuXiang
* Date: 2018-09-12 23:23
*******************************************************/

#include "NetConnectionSet.h"
#include "NetConnection.h"
#include "NetConnectionManager.h"
#include "../Macro.h"
#include <iostream>
#include <assert.h>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetConnectionSet::NetConnectionSet() : m_pManager(NULL)
		{

		}

		NetConnectionSet::~NetConnectionSet()
		{
			for (ConnectionMap::iterator itr = m_Connections.begin(); itr != m_Connections.end(); ++itr)
			{
				delete itr->second;
			}
			m_Connections.clear();
		}

		int NetConnectionSet::OnSocketRead(SOCKET s)
		{
			static Byte buff[1024];
			NetConnection *pcon = m_Connections[s];
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
						assert(m_pManager != NULL);
						m_pManager->OnRecvPackage(pcon);
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

		int NetConnectionSet::OnSocketWrite(SOCKET s)
		{
			NetConnection *pcon = m_Connections[s];
			if (pcon == NULL)
			{
				return 0;
			}
			return pcon->DoSend();
		}

		void NetConnectionSet::OnSocketClose(SOCKET s)
		{
			//连接断开
			NetConnection *pcon = m_Connections[s];
			if (pcon == NULL)
			{
				return;
			}
			if (m_pManager != NULL)
			{
				m_pManager->OnRemoveConnection(pcon);
			}
			m_Connections.erase(pcon->GetSocket());
			::closesocket(pcon->GetSocket());
			pcon->SetSocket(SOCKET_ERROR);
			SAFE_DELETE(pcon);
		}

		void NetConnectionSet::SelectSocket()
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
			for (ConnectionMap::iterator itr = m_Connections.begin(); itr != m_Connections.end(); ++itr)
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
				for (ConnectionMap::iterator itr = m_Connections.begin(); itr != m_Connections.end(); ++itr)
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

		NetConnection* NetConnectionSet::AddConnection(SOCKET s)
		{
			if (m_Connections.size() >= MAX_CONNECTION_NUMBER)
			{
				return NULL;
			}
			assert(m_Connections.find(s) == m_Connections.end());

			static UInt64 cur_uid = 0;
			NetConnection *pcon = new NetConnection();
			pcon->SetUniqueID(++cur_uid);
			pcon->SetSocket(s);
			m_Connections.insert(ConnectionMap::value_type(s, pcon));

			return pcon;
		}

		NetConnection* NetConnectionSet::GetConnection(SOCKET s)
		{
			ConnectionMap::iterator itr = m_Connections.find(s);
			return itr == m_Connections.end() ? NULL : itr->second;
		}

		void NetConnectionSet::CloseConnection(NetConnection *con)
		{
			assert(con != NULL);
			OnSocketClose(con->GetSocket());
		}
	}
}