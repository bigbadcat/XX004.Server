#include "NetTest.h"
#include <iostream>
#include <string>
#include "Net/NetServer.h"

using namespace std;

namespace XX004
{
	NetTest::NetTest() : m_running(false)
	{

	}

	NetTest::~NetTest()
	{
		Stop();
	}

	void NetTest::Run()
	{
		//cout << "NetTest::DoTest()" << endl;

		//
		//Net::NetServer netserver;
		//netserver.Start("127.0.0.1", 9000);

		//初始化Socket
		WORD wVersionRequested;
		WSADATA wsaData;
		int32_t err;
		wVersionRequested = MAKEWORD(2, 2);
		err = ::WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			cout << "WSAStartup err:" << err << endl;
			return;
		}

		m_running = true;
		m_listen_thread = thread(ListenFun, this);
	}

	void NetTest::Stop()
	{
		if (!m_running)
		{
			return;
		}

		m_running = false;
		if (m_listen_thread.joinable())
		{
			m_listen_thread.join();
		}
		if (m_client_thread.joinable())
		{
			m_client_thread.join();
		}
		::WSACleanup();
	}

	void NetTest::ListenFun(NetTest *test)
	{
		//创建socket开始监听
		SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
		if (s == SOCKET_ERROR)
		{
			cout << "create socket err:" << WSAGetLastError() << endl;
			return;
		}

		//开始绑定
		int port = 9000;
		sockaddr_in addr_in;
		::memset(&addr_in, 0, sizeof(addr_in));
		addr_in.sin_family = AF_INET;
		addr_in.sin_addr.s_addr = ::htonl(INADDR_ANY);
		addr_in.sin_port = ::htons(port);
		int ret = ::bind(s, (const sockaddr *)&addr_in, sizeof(addr_in));
		if (ret != 0)
		{
			cout << "bind socket err:" << WSAGetLastError() << endl;
			::closesocket(s);
			return;
		}

		//开始监听
		ret = ::listen(s, 64);
		if (ret != 0)
		{
			cout << "listen socket err:" << WSAGetLastError() << endl;
			::closesocket(s);
			return;
		}

		//非阻塞
		bool noblocking = true;
		u_long argp = noblocking ? 1 : 0;
		ret = ::ioctlsocket(s, FIONBIO, &argp);
		if (ret != 0)
		{
			cout << "set socket noblocking err:" << WSAGetLastError() << endl;
			::closesocket(s);
			return;
		}

		//循环检测是否有连接
		fd_set readfds;
		fd_set writefds;
		fd_set exceptfds;
		while (test->m_running)
		{
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
			FD_SET(s, &readfds);
			//FD_SET(s, &writefds);		//监听socket不写入
			FD_SET(s, &exceptfds);

			timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			int ret = ::select(0, &readfds, &writefds, &exceptfds, &timeout);		//windows下nfds参数无用，可传入0
			if (ret > 0)
			{
				//先判断是否有异常
				if (FD_ISSET(s, &exceptfds) != 0)
				{
					cout << "socket except" << endl;
					break;
				}
				if (FD_ISSET(s, &readfds) != 0)
				{
					sockaddr_in r_addr;
					int addr_size = sizeof(r_addr);
					::memset(&r_addr, 0, sizeof(r_addr));
					SOCKET rs = ::accept(s, (sockaddr*)&r_addr, &addr_size);
					if (rs == SOCKET_ERROR)
					{
						cout << "accept socket err:" << WSAGetLastError() << endl;
						break;
					}

					char ip[32];
					::sprintf_s(ip, "%d.%d.%d.%d", r_addr.sin_addr.s_net, r_addr.sin_addr.s_host, r_addr.sin_addr.s_lh, r_addr.sin_addr.s_impno);
					cout << "accept connet ip:" << ip << " port:" << r_addr.sin_port << endl;
					cout << "close connection in 5 second......" << endl;


					::Sleep(5000);
					cout << "close connection." << endl;
					::closesocket(rs);

					//test->m_client_thread = thread(ListenFun, test, rs);
				}
			}
			else if (ret == 0)
			{
				//没有消息 再等100毫秒后检测
				::Sleep(100);
			}
			else if (ret == SOCKET_ERROR)
			{
				cout << "select socket err:" << WSAGetLastError() << endl;
				break;
			}
		}

		//监听结束
		::closesocket(s);
	}

	void NetTest::ClientFun(NetTest *test)
	{
		//int recvbuffsize = 8 * 1024;
		//int sendbuffsize = 8 * 1024;
		//::setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char *)&recvbuffsize, sizeof(int));
		//::setsockopt(s, SOL_SOCKET, SO_SNDBUF, (const char *)&sendbuffsize, sizeof(int));

		//::closesocket(s);
	}
}