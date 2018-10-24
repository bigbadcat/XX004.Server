#include "NetServer.h"
#include "NetListener.h"
#include "NetConnectionManager.h"
#include "../Macro.h"

namespace XX004
{
	namespace Net
	{
		NetServer::NetServer() : m_pListener(NULL), m_pConnectionManager(NULL)
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
			//cout << "NetServer::Start ipaddress:" << ipaddress << " port:" << port << endl;
			m_pListener->Start(ipaddress, port);
		}

		void NetServer::Stop()
		{
			m_pListener->Stop();
			m_pConnectionManager->Stop();
			m_pListener->Join();
			m_pConnectionManager->Join();
		}

		void NetServer::OnConnect(SOCKET s)
		{
			//添加到连接管理器
			if (m_pConnectionManager != NULL)
			{
				m_pConnectionManager->AddConnection(s);
			}
		}

		void NetServer::OnDisconnect(NetConnection* con)
		{
			cout << "Disconnect ip:" << con->GetIPAddress() << " port:" << con->GetPort() << endl;
		}
	}
}