#include "NetListener.h"
#include <assert.h>

namespace XX004
{
	namespace Net
	{
		NetListener::NetListener() : m_Port(0)
		{
		}

		NetListener::~NetListener()
		{
		}

		void NetListener::Start(string ipaddress, int port)
		{
			m_IPAddress = ipaddress;
			m_Port = port;
			NetSocketThread::Start();
		}

		int NetListener::OnSocketRead(NetSocketWrap *wrap)
		{
			return 0;
		}

		int NetListener::OnSocketWrite(NetSocketWrap *wrap)
		{
			return 0;
		}

		void NetListener::OnSocketClose(NetSocketWrap *wrap)
		{
		}

		void NetListener::OnBegin()
		{
			assert(m_IPAddress.length() > 0 && m_Port > 0);

			//创建监听用的socket
		}

		//线程结束
		void NetListener::OnEnd()
		{
		}
	}
}