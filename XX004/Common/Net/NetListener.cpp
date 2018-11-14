﻿/*******************************************************
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
#include "NetDefine.h"
#include <assert.h>
#include <iostream>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetListener::NetListener() : m_Socket(SOCKET_ERROR), m_Port(0), m_pServer(NULL)
		{
		}

		NetListener::~NetListener()
		{
			SAFE_CLOSE_SOCKET(m_Socket)
		}

		void NetListener::Start(int port)
		{
			cout << "NetListener::Start port:" << port << endl;
			m_Port = port;			
			assert(m_Port > 0);
			assert(m_Socket == SOCKET_ERROR);
			m_Socket = CreateListenSocket();
			assert(m_Socket != SOCKET_ERROR);
		}

		void NetListener::Stop()
		{
			cout << "NetListener::Stop"<< endl;
			SAFE_CLOSE_SOCKET(m_Socket)
		}

		void NetListener::Select()
		{
			if (m_Socket == SOCKET_ERROR)
			{
				return;
			}

			fd_set readfds;
			fd_set writefds;
			fd_set exceptfds;
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
			FD_SET(m_Socket, &readfds);
			FD_SET(m_Socket, &exceptfds);
			timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			int ret = ::select(0, &readfds, &writefds, &exceptfds, &timeout);		//windows下nfds参数无用，可传入0
			if (ret > 0)
			{
				//先判断是否有异常
				if (FD_ISSET(m_Socket, &exceptfds) != 0)
				{
					cout << "listener socket exception" << endl;
					SAFE_CLOSE_SOCKET(m_Socket)
				}
				if (FD_ISSET(m_Socket, &readfds) != 0)
				{
					if (OnSocketRead() != 0)
					{
						cout << "listener socket read error" << endl;
						SAFE_CLOSE_SOCKET(m_Socket)
					}
				}
			}
			else if (ret == SOCKET_ERROR)
			{
				cout << "select listener socket err:" << WSAGetLastError() << endl;
				SAFE_CLOSE_SOCKET(m_Socket)
			}
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

			//非阻塞
			bool noblocking = true;
			u_long argp = noblocking ? 1 : 0;
			ret = ::ioctlsocket(s, FIONBIO, &argp);
			if (ret != 0)
			{
				cout << "set listen socket noblocking err:" << WSAGetLastError() << endl;
				::closesocket(s);
				return SOCKET_ERROR;
			}
			return s;
		}

		int NetListener::OnSocketRead()
		{
			sockaddr_in r_addr;
			int addr_size = sizeof(r_addr);
			::memset(&r_addr, 0, sizeof(r_addr));
			SOCKET rs = ::accept(m_Socket, (sockaddr*)&r_addr, &addr_size);
			if (rs == SOCKET_ERROR)
			{
				cout << "accept socket err:" << WSAGetLastError() << endl;
				return 1;
			}

			//非阻塞
			bool noblocking = true;
			u_long argp = noblocking ? 1 : 0;
			int ret = ::ioctlsocket(rs, FIONBIO, &argp);
			if (ret != 0)
			{
				cout << "set remote socket noblocking err:" << WSAGetLastError() << endl;
				::closesocket(rs);
				return 2;
			}

			struct linger so_linger;
			so_linger.l_onoff = 1;
			so_linger.l_linger = 5;
			ret = ::setsockopt(rs, SOL_SOCKET, SO_LINGER, (char*)&so_linger, sizeof so_linger);
			if (ret != 0)
			{
				cout << "setsockopt err:" << WSAGetLastError() << endl;
				::closesocket(rs);
				return 3;
			}

			if (m_pServer != NULL)
			{
				m_pServer->OnConnect(rs);
			}

			return 0;
		}
	}
}