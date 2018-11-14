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
#include "NetConnectionSet.h"
#include <assert.h>

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
			for (ConnectionVector::iterator itr = m_ConnectionSets.begin(); itr != m_ConnectionSets.end(); ++itr)
			{
				delete (*itr);
			}
			m_ConnectionSets.clear();
		}

		void NetConnectionManager::SelectSocket()
		{
			for (ConnectionVector::iterator itr = m_ConnectionSets.begin(); itr != m_ConnectionSets.end(); ++itr)
			{
				(*itr)->SelectSocket();
			}
		}

		//void NetConnectionManager::Start()
		//{
		//	for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); itr != m_ConnectionThreads.end(); ++itr)
		//	{
		//		(*itr)->Start();
		//	}
		//}

		//void NetConnectionManager::Stop()
		//{
		//	for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); itr != m_ConnectionThreads.end(); ++itr)
		//	{
		//		(*itr)->Stop();
		//	}
		//}

		//void NetConnectionManager::Join()
		//{
		//	for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); itr != m_ConnectionThreads.end(); ++itr)
		//	{
		//		(*itr)->Join();
		//	}
		//}

		NetConnection* NetConnectionManager::AddConnection(SOCKET s)
		{
			//找一个集合加入连接
			NetConnection *ret = NULL;
			for (ConnectionVector::iterator itr = m_ConnectionSets.begin(); ret == NULL && itr != m_ConnectionSets.end(); ++itr)
			{
				ret = (*itr)->AddConnection(s);
			}

			//没有线程可以加入集合了，创建新的线程
			if (ret == NULL)
			{
				NetConnectionSet *connection_set = new NetConnectionSet();
				m_ConnectionSets.push_back(connection_set);
				connection_set->SetManager(this);
				ret = connection_set->AddConnection(s);
			}

			return ret;
		}

		void NetConnectionManager::OnRemoveConnection(NetConnection* con)
		{
			if (m_pServer != NULL)
			{
				m_pServer->OnDisconnect(con);
			}
		}

		NetConnection* NetConnectionManager::GetConnection(SOCKET s)
		{
			//逐个线程查找
			NetConnection *ret = NULL;
			for (ConnectionVector::iterator itr = m_ConnectionSets.begin(); ret == NULL && itr != m_ConnectionSets.end(); ++itr)
			{
				ret = (*itr)->GetConnection(s);
			}
			return ret;
		}

		//void NetConnectionManager::SetRemote(NetConnection* con, const RemoteKey& key)
		//{
		//	con->SetRemote(key);
		//	m_RemoteToConnection[key] = con;
		//}

		//NetConnection* NetConnectionManager::GetConnection(const RemoteKey& key)
		//{
		//	ConnectionMap::iterator itr = m_RemoteToConnection.find(key);
		//	return itr == m_RemoteToConnection.end() ? NULL : itr->second;
		//}

		void NetConnectionManager::OnRecvPackage(NetConnection *con)
		{
			assert(con != NULL && m_pServer != NULL);
			m_pServer->OnRecvData(con);
		}
	}
}