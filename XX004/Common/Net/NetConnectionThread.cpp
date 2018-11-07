/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnectionThread.cpp
* Summary: 网络通信模块服务端连接线程。
*
* Author: XuXiang
* Date: 2018-09-12 23:23
*******************************************************/

#include "NetConnectionThread.h"
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
		NetConnectionThread::NetConnectionThread() : m_pManager(NULL)
		{

		}

		NetConnectionThread::~NetConnectionThread()
		{
			for (ConnectionMap::iterator itr = m_Connections.begin(); itr != m_Connections.end(); ++itr)
			{
				delete itr->second;
			}
			m_Connections.clear();
		}

		int NetConnectionThread::OnSocketRead(NetSocketWrap *wrap)
		{
			Byte buff[1024];
			int len = ::recv(wrap->GetSocket(), (char*)buff, 1024, 0);
			if (len > 0)
			{
				NetConnection *con = dynamic_cast<NetConnection*>(wrap);
				assert(con != NULL);
				if (!con->AddRecvData(buff, len))
				{
					return 3;
				}

				do
				{
					int ret = con->CheckRecvPackage();
					if (ret == 0)
					{
						break;
					}
					else if (ret == 1)
					{
						assert(m_pManager != NULL);
						m_pManager->OnRecvPackage(con);
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

		int NetConnectionThread::OnSocketWrite(NetSocketWrap *wrap)
		{
			return 0;
		}

		void NetConnectionThread::OnSocketClose(NetSocketWrap *wrap)
		{
			//连接断开
			NetConnection *pcon = dynamic_cast<NetConnection*>(wrap);
			assert(pcon != NULL);
			if (m_pManager != NULL)
			{
				m_pManager->RemoveConnection(dynamic_cast<NetConnection*>(wrap));
			}
			m_Connections.erase(wrap->GetSocket());
			SAFE_DELETE(wrap);
		}

		void NetConnectionThread::OnBegin()
		{

		}

		void NetConnectionThread::OnEnd()
		{

		}

		NetConnection* NetConnectionThread::AddConnection(SOCKET s)
		{
			if (m_Connections.size() >= MAX_CONNECTION_NUMBER)
			{
				return NULL;
			}
			assert(m_Connections.find(s) == m_Connections.end());

			NetConnection *pcon = new NetConnection();
			pcon->SetSocket(s);
			m_Connections.insert(ConnectionMap::value_type(s, pcon));
			this->AddSocket(pcon);

			return pcon;
		}

		NetConnection* NetConnectionThread::GetConnection(SOCKET s)
		{
			ConnectionMap::iterator itr = m_Connections.find(s);
			return itr == m_Connections.end() ? NULL : itr->second;
		}
	}
}