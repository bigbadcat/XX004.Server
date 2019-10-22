/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetServer.cpp
* Summary: 网络通信模块服务端。
*
* Author: XuXiang
* Date: 2018-07-30 22:56
*******************************************************/

#include "NetServer.h"
#include "NetConnection.h"
#include "NetConnectionManager.h"
#include "../Macro.h"
#include <iostream>
#include <string>
#include <assert.h>

namespace XX004
{
	namespace Net
	{
		NetServer::NetServer() : m_pConnectionManager(NULL), m_pProcesser(NULL)
		{
			m_pConnectionManager = new NetConnectionManager();
			m_pConnectionManager->SetServer(this);
		}

		NetServer::~NetServer()
		{
			SAFE_DELETE(m_pConnectionManager)
		}

		void NetServer::Start(int port)
		{
			::printf_s("NetServer::Start port:%d\n", port);
			m_pConnectionManager->Init(port);
		}

		void NetServer::Stop()
		{
			::printf_s("NetServer::Stop\n");
			m_pConnectionManager->Release();
		}

		void NetServer::SelectSocket(int msec)
		{
			m_pConnectionManager->SelectSocket(msec);
		}

		NetConnection* NetServer::GetConnection(UInt64 uid)
		{
			return m_pConnectionManager->GetConnection(uid);
		}

		NetConnection* NetServer::GetConnection(const RemoteKey &key)
		{
			return m_pConnectionManager->GetConnection(key);
		}

		void NetServer::SetRemote(UInt64 uid, const RemoteKey &key)
		{
			m_pConnectionManager->SetRemote(uid, key);
		}

		void NetServer::CloseConnection(NetConnection* con)
		{
			m_pConnectionManager->RemoveConnection(con);
		}

		void NetServer::OnConnect(NetConnection* con)
		{
			::printf_s("OnConnect ip:%s port:%d\n", con->GetIPAddress().c_str(), con->GetPort());
			if (m_pProcesser != NULL)
			{
				m_pProcesser->OnConnected(con);
			}
		}

		void NetServer::OnDisconnect(NetConnection* con)
		{
			::printf_s("OnDisconnect ip:%s port:%d\n", con->GetIPAddress().c_str(), con->GetPort());
			if (m_pProcesser != NULL)
			{
				m_pProcesser->OnDisconnected(con);
			}
		}

		void NetServer::OnRecvData(NetConnection* con)
		{
			static Byte buffer[NET_PACKAGE_MAX_SIZE];
			NetPackageHeader header;
			assert(con != NULL);
			con->TakeRecvPackage(header, buffer, NET_PACKAGE_MAX_SIZE);
			if (m_pProcesser != NULL)
			{
				m_pProcesser->OnRecvData(con, header, buffer);
			}
		}
	}
}