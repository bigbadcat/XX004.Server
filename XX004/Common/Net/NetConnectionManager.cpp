#include "NetConnectionManager.h"
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
				//NetConnectionThread * connection_thread = new NetConnectionThread();
			}

			return ret;
		}
	}
}