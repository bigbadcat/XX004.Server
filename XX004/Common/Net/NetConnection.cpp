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
#include "../Util/DataUtil.h"
#include <iostream>
#include <string>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetConnection::NetConnection() : m_Socket(SOCKET_ERROR), m_Port(0), m_SendLen(0), m_RecvLen(0)
		{
		}
		
		NetConnection::~NetConnection()
		{
			SAFE_CLOSE_SOCKET(m_Socket);
		}

		bool NetConnection::IsNeedWrite()
		{
			return m_SendLen > 0;
		}

		void NetConnection::SetSocket(SOCKET s)
		{
			m_Socket = s;
			m_IPAddress = "";
			m_Port = 0;
			if (s == SOCKET_ERROR)
			{
				return;
			}

			SOCKADDR addr;
			int addr_len = sizeof(addr);
			ZeroMemory(&addr, addr_len);
			int ret = getpeername(m_Socket, &addr, &addr_len);
			if (ret == 0)
			{
				if (addr.sa_family == AF_INET)
				{
					SOCKADDR_IN *addr_v4 = (PSOCKADDR_IN)&addr;
					char ip[32];
					::sprintf_s(ip, "%d.%d.%d.%d", addr_v4->sin_addr.s_net, addr_v4->sin_addr.s_host, addr_v4->sin_addr.s_lh, addr_v4->sin_addr.s_impno);
					m_IPAddress = ip;
					m_Port = addr_v4->sin_port;
					//cout << "accept connet ip:" << m_IPAddress << " port:" << m_Port << endl;
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

		bool NetConnection::AddSendData(Byte *buffer, int len)
		{
			//数据超过缓冲区大小了，不能添加
			if (m_SendLen + len > NET_BUFFER_SIZE)
			{
				return false;
			}

			::memcpy_s(m_SendBuffer + m_SendLen, NET_BUFFER_SIZE - m_SendLen, buffer, len);
			m_SendLen += len;
			return true;
		}

		bool NetConnection::AddRecvData(Byte *buffer, int len)
		{
			//数据超过缓冲区大小了，不能写入
			if (m_RecvLen + len > NET_BUFFER_SIZE)
			{
				return false;
			}

			::memcpy_s(m_RecvBuffer + m_RecvLen, NET_BUFFER_SIZE - m_RecvLen, buffer, len);
			m_RecvLen += len;

			return true;
		}

		int NetConnection::CheckRecvPackage()
		{
			if (m_RecvHeader.Sign == 0)
			{
				if (m_RecvLen >= NetPackageHeader::HEADER_SIZE)
				{
					m_RecvHeader.Unpack(m_RecvBuffer, 0);
					if (m_RecvHeader.Sign != NetPackageHeader::HEADER_SIGN || m_RecvHeader.BodySize > NET_PACKAGE_MAX_SIZE)
					{
						return -1;
					}
				}
			}

			bool have = m_RecvHeader.Sign != 0 && m_RecvLen >= (m_RecvHeader.BodySize + NetPackageHeader::HEADER_SIZE);
			return have ? 1 : 0;
		}

		void NetConnection::TakeRecvPackage(NetPackageHeader& header, Byte *buffer, int size)
		{
			//获取数据包
			int datasize = m_RecvHeader.BodySize + NetPackageHeader::HEADER_SIZE;
			header = m_RecvHeader;
			::memcpy_s(buffer, NET_PACKAGE_MAX_SIZE, m_RecvBuffer + NetPackageHeader::HEADER_SIZE, m_RecvHeader.BodySize);

			//清除数据包
			m_RecvLen -= datasize;
			::memcpy_s(m_RecvBuffer, NET_BUFFER_SIZE, m_RecvBuffer + datasize, m_RecvLen);		//数据回移
			m_RecvHeader.Reset();
		}

		int NetConnection::DoSend()
		{
			if (m_SendLen > 0)
			{
				int ret = ::send(GetSocket(), (char*)m_SendBuffer, m_SendLen, 0);
				if (ret > 0)
				{
					m_SendLen -= ret;
					if (m_SendLen > 0)
					{
						::memcpy_s(m_SendBuffer, NET_BUFFER_SIZE, m_SendBuffer + ret, m_SendLen);	//数据回移
					}					
				}
				else if (ret == SOCKET_ERROR)
				{
					cout << "send socket err:" << WSAGetLastError() << endl;
					return 1;
				}
			}
			
			return 0;
		}
	}
}