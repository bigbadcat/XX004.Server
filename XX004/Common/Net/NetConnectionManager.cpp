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
			m_ConnectionIndex.clear();
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
			m_ConnectionIndex.insert(NetConnectionMap::value_type(ret->GetUniqueID(), ret));

			return ret;
		}

		void NetConnectionManager::RemoveConnection(NetConnection* con)
		{
			SOCKET s = con->GetSocket();
			m_ConnectionIndex.erase(con->GetUniqueID());
			for (ConnectionVector::iterator itr = m_ConnectionSets.begin(); itr != m_ConnectionSets.end(); ++itr)
			{
				NetConnectionSet *pset = *itr;
				if (pset->IsContain(s) != NULL)
				{
					pset->CloseConnection(con);
				}
			}
		}

		void NetConnectionManager::OnRemoveConnection(NetConnection* con)
		{
			if (m_pServer != NULL)
			{
				m_pServer->OnDisconnect(con);
			}
		}

		NetConnection* NetConnectionManager::GetConnection(UInt64 uid)
		{
			NetConnection *ret = m_ConnectionIndex[uid];
			return ret;
		}

		void NetConnectionManager::OnRecvPackage(NetConnection *con)
		{
			assert(con != NULL && m_pServer != NULL);
			m_pServer->OnRecvData(con);
		}
	}
}