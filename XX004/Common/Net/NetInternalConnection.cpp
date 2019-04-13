/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetInternalConnection.cpp
* Summary: 网络通信模块服务内部连接。
*
* Author: XuXiang
* Date: 2018-11-26 14:49
*******************************************************/

#include "NetInternalConnection.h"
#include "../Util/TimeUtil.h"
#include <iostream>
#include <string>
#include <assert.h>
#include <WS2tcpip.h>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetInternalConnection::NetInternalConnection() : m_State(ConnectionState::CS_NOT_CONNECTED), m_RemoteType(RemoteType::RT_UNKNOW), m_Socket(SOCKET_ERROR),
			m_Port(0), m_SendBuffer(NET_BUFFER_SIZE), m_RecvBuffer(NET_BUFFER_SIZE)
		{
		}
		
		NetInternalConnection::~NetInternalConnection()
		{
			SAFE_CLOSE_SOCKET(m_Socket);
		}

		void NetInternalConnection::Init(int type, const string &ip, int port)
		{
			assert(m_State == ConnectionState::CS_NOT_CONNECTED);
			m_RemoteType = type;
			m_IPAddress = ip;
			m_Port = port;
		}

		void NetInternalConnection::Close()
		{
			m_State = ConnectionState::CS_NOT_CONNECTED;
			SAFE_CLOSE_SOCKET(m_Socket);
			m_RecvHeader.Reset();
			m_SendBuffer.Clear();
			m_RecvBuffer.Clear();
		}

		bool NetInternalConnection::Send(Int64 guid, Int32 cmd, Byte *buffer, int len)
		{
			NetPackageHeader sendhead;
			sendhead.SetSign();
			sendhead.Command = cmd;
			sendhead.GUID = guid;
			sendhead.BodySize = len;

			static Byte sendbuff[1024];
			int sendsize = 0;
			sendsize = sendhead.Pack(sendbuff, sendsize);
			::memcpy_s(sendbuff + sendsize, 1024 - sendsize, buffer, len);
			sendsize += len;

			return AddSendData(sendbuff, sendsize);
		}

		bool NetInternalConnection::AddSendData(Byte *buffer, int len)
		{
			return m_SendBuffer.AddData(buffer, len);
		}

		bool NetInternalConnection::AddRecvData(Byte *buffer, int len)
		{
			return m_RecvBuffer.AddData(buffer, len);
		}

		int NetInternalConnection::CheckRecvPackage()
		{
			if (m_RecvHeader.Sign == 0)
			{
				if (m_RecvBuffer.GetLength() >= NetPackageHeader::HEADER_SIZE)
				{
					m_RecvHeader.Unpack(m_RecvBuffer.GetData(), 0);
					if (m_RecvHeader.Sign != NetPackageHeader::HEADER_SIGN || m_RecvHeader.BodySize > NET_PACKAGE_MAX_SIZE)
					{
						return -1;
					}
				}
			}

			bool have = m_RecvHeader.Sign != 0 && m_RecvBuffer.GetLength() >= (m_RecvHeader.BodySize + NetPackageHeader::HEADER_SIZE);
			return have ? 1 : 0;
		}

		void NetInternalConnection::TakeRecvPackage(NetPackageHeader& header, Byte *buffer, int size)
		{
			//获取数据包
			int datasize = m_RecvHeader.BodySize + NetPackageHeader::HEADER_SIZE;
			header = m_RecvHeader;
			m_RecvBuffer.GetData(buffer, NetPackageHeader::HEADER_SIZE, m_RecvHeader.BodySize);

			//清除数据包
			m_RecvBuffer.RemoveData(datasize);
			m_RecvHeader.Reset();
		}

		int NetInternalConnection::DoSend()
		{
			if (m_SendBuffer.GetLength() > 0)
			{
				int ret = ::send(GetSocket(), (char*)m_SendBuffer.GetData(), m_SendBuffer.GetLength(), 0);
				if (ret > 0)
				{		
					m_SendBuffer.RemoveData(ret);
				}
				else if (ret == SOCKET_ERROR)
				{
					cout << "send socket err:" << WSAGetLastError() << endl;
					return 1;
				}
			}
			
			return 0;
		}

		void NetInternalConnection::Select()
		{
			if (m_State == ConnectionState::CS_CONNECTED)
			{
				CheckIO();
			}
			else if (m_State == ConnectionState::CS_NOT_CONNECTED)
			{
				if (m_IPAddress.length() > 0 && m_Port > 0)
				{
					StartConnect();
				}
			}
			else if (m_State == ConnectionState::CS_CONNECTING)
			{
				CheckConnect();
			}
			else if (m_State == ConnectionState::CS_RETRY_WAITTING)
			{
				if (TimeUtil::GetCurrentSecond() >= m_RetryTimestamp)
				{
					m_State = ConnectionState::CS_NOT_CONNECTED;
				}
			}
		}

		void NetInternalConnection::StartConnect()
		{
			cout << "NetInternalConnection::StartConnect ip:" << m_IPAddress << " port:" << m_Port << endl;

			SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
			if (s == SOCKET_ERROR)
			{
				cout << "create socket err:" << WSAGetLastError() << endl;
				return;
			}

			SOCKADDR_IN addrSrv;
			int pton = inet_pton(AF_INET, m_IPAddress.c_str(), &addrSrv.sin_addr.S_un.S_addr);
			addrSrv.sin_family = AF_INET;
			addrSrv.sin_port = htons((u_short)m_Port);

			//非阻塞
			bool noblocking = true;
			u_long argp = noblocking ? 1 : 0;
			int ret = ::ioctlsocket(s, FIONBIO, &argp);
			if (ret != 0)
			{
				cout << "set listen socket noblocking err:" << WSAGetLastError() << endl;
				::closesocket(s);
				return;
			}
			::connect(s, (sockaddr *)&addrSrv, sizeof (sockaddr));
			m_Socket = s;
			m_State = ConnectionState::CS_CONNECTING;
		}

		void NetInternalConnection::CheckConnect()
		{
			fd_set readfds;
			fd_set writefds;
			fd_set exceptfds;
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
			FD_SET(m_Socket, &writefds);
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
					cout << "InternalConnection connect failed. ip:" << m_IPAddress << " port:" << m_Port << endl;
					SAFE_CLOSE_SOCKET(m_Socket);
					Retry();
				}
				//可以写入了则说明已经连接上
				if (FD_ISSET(m_Socket, &writefds) != 0)
				{
					m_State = ConnectionState::CS_CONNECTED;
				}
			}
			else if (ret == SOCKET_ERROR)
			{
				cout << "InternalConnection connect error:" << WSAGetLastError() << endl;
				SAFE_CLOSE_SOCKET(m_Socket)
				Retry();
			}
		}

		void NetInternalConnection::CheckIO()
		{
			fd_set readfds;
			fd_set writefds;
			fd_set exceptfds;
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
			if (IsNeedRead())
			{
				FD_SET(m_Socket, &readfds);
			}
			if (IsNeedWrite())
			{
				FD_SET(m_Socket, &writefds);
			}			
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
					m_State = ConnectionState::CS_NOT_CONNECTED;
					cout << "InternalConnection socket exception" << endl;
					SAFE_CLOSE_SOCKET(m_Socket);
				}

				if (FD_ISSET(m_Socket, &readfds) != 0)
				{
					if (OnSocketRead() != 0)
					{
						m_State = ConnectionState::CS_NOT_CONNECTED;
						cout << "InternalConnection socket read error" << endl;
						SAFE_CLOSE_SOCKET(m_Socket)
					}
				}
				if (FD_ISSET(m_Socket, &writefds) != 0)
				{
					if (OnSocketWrite() != 0)
					{
						m_State = ConnectionState::CS_NOT_CONNECTED;
						cout << "InternalConnection socket write error" << endl;
						SAFE_CLOSE_SOCKET(m_Socket)
					}
				}
			}
			else if (ret == SOCKET_ERROR)
			{
				m_State = ConnectionState::CS_NOT_CONNECTED;
				cout << "select InternalConnection socket err:" << WSAGetLastError() << endl;
				SAFE_CLOSE_SOCKET(m_Socket)
			}
		}

		int NetInternalConnection::OnSocketRead()
		{
			static Byte buff[1024];
			int len = ::recv(m_Socket, (char*)buff, 1024, 0);
			if (len > 0)
			{
				if (!AddRecvData(buff, len))
				{
					return 3;
				}
			}
			else if (len == 0)
			{
				//客户端关闭了连接
				return 1;
			}
			else
			{
				cout << "recv socket err:" << WSAGetLastError() << endl;
				return 2;
			}

			return 0;
		}

		int NetInternalConnection::OnSocketWrite()
		{
			return DoSend();			
		}

		void NetInternalConnection::Retry()
		{
			m_RetryTimestamp = TimeUtil::GetCurrentSecond() + RETRY_WAITTING_SECONDS;
			m_State = ConnectionState::CS_RETRY_WAITTING;
		}
	}
}