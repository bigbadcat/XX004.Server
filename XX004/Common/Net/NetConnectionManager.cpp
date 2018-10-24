﻿#include "NetConnectionManager.h"
#include "NetServer.h"
#include "NetConnectionThread.h"
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
			Stop();
			Join();
			for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); itr != m_ConnectionThreads.end(); ++itr)
			{
				delete (*itr);
			}
			m_ConnectionThreads.clear();
		}

		void NetConnectionManager::Start()
		{
			for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); itr != m_ConnectionThreads.end(); ++itr)
			{
				(*itr)->Start();
			}
		}

		void NetConnectionManager::Stop()
		{
			for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); itr != m_ConnectionThreads.end(); ++itr)
			{
				(*itr)->Stop();
			}
		}

		void NetConnectionManager::Join()
		{
			for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); itr != m_ConnectionThreads.end(); ++itr)
			{
				(*itr)->Join();
			}
		}

		NetConnection* NetConnectionManager::AddConnection(SOCKET s)
		{
			//找一个线程加入连接
			NetConnection *ret = NULL;
			for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); ret == NULL && itr != m_ConnectionThreads.end(); ++itr)
			{
				ret = (*itr)->AddConnection(s);
			}

			//没有线程可以加入连接了，创建新的线程
			if (ret == NULL)
			{
				NetConnectionThread *connection_thread = new NetConnectionThread();
				m_ConnectionThreads.push_back(connection_thread);
				connection_thread->SetManager(this);
				connection_thread->Start();
				ret = connection_thread->AddConnection(s);
			}

			return ret;
		}

		void NetConnectionManager::RemoveConnection(NetConnection* con)
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
			for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); ret == NULL && itr != m_ConnectionThreads.end(); ++itr)
			{
				ret = (*itr)->GetConnection(s);
			}
			return ret;
		}

		NetConnection* NetConnectionManager::GetConnection(const RemoteKey& key)
		{
			return NULL;
		}
	}
}