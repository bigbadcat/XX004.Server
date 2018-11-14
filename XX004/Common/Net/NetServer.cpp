﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetServer.cpp
* Summary: 网络通信模块服务端。
*
* Author: XuXiang
* Date: 2018-07-30 22:56
*******************************************************/

#include "NetServer.h"
#include "NetListener.h"
#include "NetConnectionManager.h"
#include "../Macro.h"
#include <assert.h>

namespace XX004
{
	namespace Net
	{
		NetServer::NetServer() : m_pListener(NULL), m_pConnectionManager(NULL), m_pProcesser(NULL)
		{
			m_pListener = new NetListener();
			m_pListener->SetServer(this);
			m_pConnectionManager = new NetConnectionManager();
			m_pConnectionManager->SetServer(this);
		}

		NetServer::~NetServer()
		{
			SAFE_DELETE(m_pListener)
			SAFE_DELETE(m_pConnectionManager)
		}

		void NetServer::Start(const string &ipaddress, int port)
		{
			cout << "NetServer::Start ipaddress:" << ipaddress << " port:" << port << endl;
			m_pListener->Start(port);
			m_pConnectionManager->Init();
		}

		void NetServer::Stop()
		{
			cout << "NetServer::Stop" << endl;
			m_pListener->Stop();
			m_pConnectionManager->Release();
		}

		void NetServer::SelectSocket()
		{
			m_pListener->Select();
			m_pConnectionManager->SelectSocket();
		}
//
//		void NetServer::SetRemote(NetConnection* con, const RemoteKey& key) 
//		{ 
//			//m_pConnectionManager->SetRemote(con, key);
//		}
//
//		NetConnection* NetServer::GetConnection(const RemoteKey& key)
//		{ 
//			//return m_pConnectionManager->GetConnection(key); 
//			return NULL;
//		}
//
//		void NetServer::CloseConnection(NetConnection* con)
//		{
//			//m_pConnectionManager->RemoveConnection(con);
//		}
//
//		void NetServer::CloseConnection(const RemoteKey& key)
//		{
///*			NetConnection *con = m_pConnectionManager->GetConnection(key);
//			if (con != NULL)
//			{
//				m_pConnectionManager->RemoveConnection(con);
//			}	*/		
//		}

		void NetServer::OnConnect(SOCKET s)
		{
			//添加到连接管理器
			if (m_pConnectionManager != NULL)
			{
				//NetConnection *con = new NetConnection();
				//con->SetSocket(s);
				//cout << "OnConnect ip:" << con->GetIPAddress() << " port:" << con->GetPort() << endl;
				//cout << "close in 5 second..." << endl;

				//std::chrono::milliseconds dura(5000);
				//std::this_thread::sleep_for(dura);
				//SAFE_DELETE(con)


				NetConnection* con = m_pConnectionManager->AddConnection(s);
				cout << "OnConnect ip:" << con->GetIPAddress() << " port:" << con->GetPort() << endl;
				if (m_pProcesser != NULL)
				{
					m_pProcesser->OnConnected(con);
				}
			}
		}

		void NetServer::OnDisconnect(NetConnection* con)
		{
			cout << "OnDisconnect ip:" << con->GetIPAddress() << " port:" << con->GetPort() << endl;
			if (m_pProcesser != NULL)
			{
				m_pProcesser->OnDisconnected(con);
			}
		}

		void NetServer::OnRecvData(NetConnection* con)
		{
			static Byte buffer[NET_PACKAGE_MAX_SIZE];
			static NetPackageHeader header;
			assert(con != NULL);
			con->TakeRecvPackage(header, buffer, NET_PACKAGE_MAX_SIZE);
			if (m_pProcesser != NULL)
			{
				m_pProcesser->OnRecvData(con, header, buffer);
			}
		}
	}
}