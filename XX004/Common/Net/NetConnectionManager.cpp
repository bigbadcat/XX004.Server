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
			//��һ���̼߳�������
			NetConnection *ret = NULL;
			for (ConnectionVector::iterator itr = m_ConnectionThreads.begin(); ret == NULL && itr != m_ConnectionThreads.end(); ++itr)
			{
				ret = (*itr)->AddConnection(s);
			}

			//û���߳̿��Լ��������ˣ������µ��߳�
			if (ret == NULL)
			{
				//NetConnectionThread * connection_thread = new NetConnectionThread();
			}

			return ret;
		}
	}
}