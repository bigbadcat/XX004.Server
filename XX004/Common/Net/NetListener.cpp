/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetListener.cpp
* Summary: 网络通信模块服务端连接监听线程。
*
* Author: XuXiang
* Date: 2018-07-30 22:58
*******************************************************/

#include "NetListener.h"
#include "NetServer.h"
#include <assert.h>
#include <iostream>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetListener::NetListener() : m_Port(0), m_pServer(NULL)
		{
		}

		NetListener::~NetListener()
		{
		}

		void NetListener::Start(const string &ipaddress, int port)
		{
			m_IPAddress = ipaddress;
			m_Port = port;
			NetSocketThread::Start();
		}

		int NetListener::OnSocketRead(NetSocketWrap *wrap)
		{
			sockaddr_in r_addr;
			int addr_size = sizeof(r_addr);
			::memset(&r_addr, 0, sizeof(r_addr));
			SOCKET rs = ::accept(wrap->GetSocket(), (sockaddr*)&r_addr, &addr_size);
			if (rs == SOCKET_ERROR)
			{
				cout << "accept socket err:" << WSAGetLastError() << endl;
				return 1;
			}

			if (m_pServer != NULL)
			{
				m_pServer->OnConnect(rs);
			}

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
			if (m_ListenSocket.GetSocket() == SOCKET_ERROR)
			{
				SOCKET s = CreateListenSocket();
				if (s != SOCKET_ERROR)
				{
					m_ListenSocket.SetSocket(s);
					AddSocket(&m_ListenSocket);
				}
			}
		}

		//线程结束
		void NetListener::OnEnd()
		{
		}

		SOCKET NetListener::CreateListenSocket()
		{
			//创建socket开始监听
			SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
			if (s == SOCKET_ERROR)
			{
				cout << "create socket err:" << WSAGetLastError() << endl;
				return SOCKET_ERROR;
			}

			//开始绑定
			sockaddr_in addr_in;
			::memset(&addr_in, 0, sizeof(addr_in));
			addr_in.sin_family = AF_INET;
			addr_in.sin_addr.s_addr = ::htonl(INADDR_ANY);
			addr_in.sin_port = ::htons(m_Port);
			int ret = ::bind(s, (const sockaddr *)&addr_in, sizeof(addr_in));
			if (ret != 0)
			{
				cout << "bind socket err:" << WSAGetLastError() << endl;
				::closesocket(s);
				return SOCKET_ERROR;
			}

			//开始监听
			ret = ::listen(s, 64);
			if (ret != 0)
			{
				cout << "listen socket err:" << WSAGetLastError() << endl;
				::closesocket(s);
				return SOCKET_ERROR;
			}

			struct linger so_linger;
			so_linger.l_onoff = 1;
			so_linger.l_linger = 5;
			ret = ::setsockopt(s, SOL_SOCKET, SO_LINGER, (char*)&so_linger, sizeof so_linger);
			if (ret != 0)
			{
				cout << "setsockopt err:" << WSAGetLastError() << endl;
				::closesocket(s);
				return SOCKET_ERROR;
			}

			//非阻塞
			bool noblocking = true;
			u_long argp = noblocking ? 1 : 0;
			ret = ::ioctlsocket(s, FIONBIO, &argp);
			if (ret != 0)
			{
				cout << "set socket noblocking err:" << WSAGetLastError() << endl;
				::closesocket(s);
				return SOCKET_ERROR;
			}
			return s;
		}
	}
}