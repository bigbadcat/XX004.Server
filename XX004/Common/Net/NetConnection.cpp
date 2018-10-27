/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnection.cpp
* Summary: 网络通信模块服务端连接。
*
* Author: XuXiang
* Date: 2018-07-30 23:06
*******************************************************/

#include "NetConnection.h"
#include <iostream>
#include <string>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetConnection::NetConnection() : m_Port(0)
		{
		}
		
		NetConnection::~NetConnection()
		{
		}

		void NetConnection::SetSocket(SOCKET s)
		{
			NetSocketWrap::SetSocket(s);
			if (s == SOCKET_ERROR)
			{
				return;
			}

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
					m_IPAddress = ip;
					m_Port = addr_v4->sin_port;
					cout << "accept connet ip:" << m_IPAddress << " port:" << m_Port << endl;
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
		}
	}
}