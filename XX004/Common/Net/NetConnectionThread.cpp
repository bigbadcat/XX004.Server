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

		}

		int NetConnectionThread::OnSocketRead(NetSocketWrap *wrap)
		{
			char buff[1024];
			int len = ::recv(wrap->GetSocket(), buff, 1024, 0);
			if (len > 0)
			{

			}
			else if (len == 0)
			{
				//客户端关闭了连接
				return 1;
			}
			else
			{
				//cout << "recv socket err:" << WSAGetLastError() << endl;
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

			NetConnection *pcon = new NetConnection();
			pcon->SetSocket(s);
			this->AddSocket(pcon);

			//cout << "close connection in 5 second......" << endl;
			//::Sleep(5000);
			//cout << "close connection." << endl;
			//::closesocket(s);

			return pcon;
		}

		NetConnection* NetConnectionThread::GetConnection(SOCKET s)
		{
			ConnectionMap::iterator itr = m_Connections.find(s);
			return itr == m_Connections.end() ? NULL : itr->second;
		}
	}
}