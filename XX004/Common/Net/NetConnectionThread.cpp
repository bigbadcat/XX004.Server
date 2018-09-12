#include "NetConnectionThread.h"
#include "NetConnection.h"
#include <iostream>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetConnectionThread::NetConnectionThread()
		{

		}

		NetConnectionThread::~NetConnectionThread()
		{

		}

		NetConnection* NetConnectionThread::AddConnection(SOCKET s)
		{
			SOCKADDR addr;
			int addr_len = sizeof(addr);
			ZeroMemory(&addr, addr_len);
			int ret = getpeername(s, &addr, &addr_len);
			if (ret == 0)
			{
				if (addr.sa_family == AF_INET)
				{
					SOCKADDR_IN *addr_v4 = (PSOCKADDR_IN)&addr;
					char ip[32];
					::sprintf_s(ip, "%d.%d.%d.%d", addr_v4->sin_addr.s_net, addr_v4->sin_addr.s_host, addr_v4->sin_addr.s_lh, addr_v4->sin_addr.s_impno);
					cout << "accept connet ip:" << ip << " port:" << addr_v4->sin_port << endl;
				}
				else
				{
					cout << "socket addr.sa_family:" << addr.sa_family << endl;
				}
			}
			else
			{
				cout << "getpeername err:" << WSAGetLastError() << endl;
			}

			cout << "close connection in 5 second......" << endl;
			::Sleep(5000);
			cout << "close connection." << endl;
			::closesocket(s);

			return NULL;
		}
	}
}