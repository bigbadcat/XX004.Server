﻿#include "NetTest.h"
#include <iostream>
#include <string>
#include "Net/NetServer.h"
#include "Net/NetPackageHeader.h"
#include "Util/DataUtil.h"

using namespace std;
using namespace XX004::Net;

namespace XX004
{
	NetTest::NetTest() : m_running(false), m_client_socket(SOCKET_ERROR)
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
		m_Server.Start("127.0.0.1", 9000);
		//m_listerner.Start("127.0.0.1", 9000);
		//m_listen_thread = thread(ListenFun, this);
	}

	void NetTest::Stop()
	{
		if (!m_running)
		{
			return;
		}

		m_running = false;
		m_Server.Stop();
		//m_listerner.Join();
		//if (m_listen_thread.joinable())
		//{
		//	m_listen_thread.join();
		//}
		for (std::vector<std::thread>::iterator itr = m_client_thread.begin(); itr != m_client_thread.end(); ++itr)
		{
			if (itr->joinable())
			{
				itr->join();
			}
		}
		m_client_thread.clear();
		::WSACleanup();
	}

	//void NetTest::ListenFun(NetTest *test)
	//{
	//	//创建socket开始监听
	//	SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
	//	if (s == SOCKET_ERROR)
	//	{
	//		cout << "create socket err:" << WSAGetLastError() << endl;
	//		return;
	//	}

	//	//开始绑定
	//	int port = 9000;
	//	sockaddr_in addr_in;
	//	::memset(&addr_in, 0, sizeof(addr_in));
	//	addr_in.sin_family = AF_INET;
	//	addr_in.sin_addr.s_addr = ::htonl(INADDR_ANY);
	//	addr_in.sin_port = ::htons(port);
	//	int ret = ::bind(s, (const sockaddr *)&addr_in, sizeof(addr_in));
	//	if (ret != 0)
	//	{
	//		cout << "bind socket err:" << WSAGetLastError() << endl;
	//		::closesocket(s);
	//		return;
	//	}

	//	//开始监听
	//	ret = ::listen(s, 64);
	//	if (ret != 0)
	//	{
	//		cout << "listen socket err:" << WSAGetLastError() << endl;
	//		::closesocket(s);
	//		return;
	//	}

	//	//非阻塞
	//	bool noblocking = true;
	//	u_long argp = noblocking ? 1 : 0;
	//	ret = ::ioctlsocket(s, FIONBIO, &argp);
	//	if (ret != 0)
	//	{
	//		cout << "set socket noblocking err:" << WSAGetLastError() << endl;
	//		::closesocket(s);
	//		return;
	//	}

	//	//循环检测是否有连接
	//	fd_set readfds;
	//	fd_set writefds;
	//	fd_set exceptfds;
	//	while (test->m_running)
	//	{
	//		FD_ZERO(&readfds);
	//		FD_ZERO(&writefds);
	//		FD_ZERO(&exceptfds);
	//		FD_SET(s, &readfds);
	//		//FD_SET(s, &writefds);		//监听socket不写入
	//		FD_SET(s, &exceptfds);

	//		timeval timeout;
	//		timeout.tv_sec = 0;
	//		timeout.tv_usec = 0;
	//		int ret = ::select(0, &readfds, &writefds, &exceptfds, &timeout);		//windows下nfds参数无用，可传入0
	//		if (ret > 0)
	//		{
	//			//先判断是否有异常
	//			if (FD_ISSET(s, &exceptfds) != 0)
	//			{
	//				cout << "socket except" << endl;
	//				break;
	//			}
	//			if (FD_ISSET(s, &readfds) != 0)
	//			{
	//				sockaddr_in r_addr;
	//				int addr_size = sizeof(r_addr);
	//				::memset(&r_addr, 0, sizeof(r_addr));
	//				SOCKET rs = ::accept(s, (sockaddr*)&r_addr, &addr_size);
	//				if (rs == SOCKET_ERROR)
	//				{
	//					cout << "accept socket err:" << WSAGetLastError() << endl;
	//					break;
	//				}

	//				char ip[32];
	//				::sprintf_s(ip, "%d.%d.%d.%d", r_addr.sin_addr.s_net, r_addr.sin_addr.s_host, r_addr.sin_addr.s_lh, r_addr.sin_addr.s_impno);
	//				cout << "accept connet ip:" << ip << " port:" << r_addr.sin_port << endl;

	//				//cout << "close connection in 5 second......" << endl;
	//				//::Sleep(5000);
	//				//cout << "close connection." << endl;
	//				//::closesocket(rs);

	//				test->m_client_socket = rs;
	//				test->m_client_thread.push_back(thread(ClientFun, test));
	//			}
	//		}
	//		else if (ret == 0)
	//		{
	//			//没有消息 再等100毫秒后检测
	//			::Sleep(100);
	//		}
	//		else if (ret == SOCKET_ERROR)
	//		{
	//			cout << "select socket err:" << WSAGetLastError() << endl;
	//			break;
	//		}
	//	}

	//	//监听结束
	//	::closesocket(s);
	//}

	std::string UTF8_To_string(const std::string & str)
	{
		int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

		wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
		memset(pwBuf, 0, nwLen * 2 + 2);

		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

		int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char * pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);

		WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string retStr = pBuf;

		delete[]pBuf;
		delete[]pwBuf;

		pBuf = NULL;
		pwBuf = NULL;

		return retStr;
	}

	std::string string_To_UTF8(const std::string & str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

		wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
		ZeroMemory(pwBuf, nwLen * 2 + 2);

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char * pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);

		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string retStr(pBuf);

		delete[]pwBuf;
		delete[]pBuf;

		pwBuf = NULL;
		pBuf = NULL;

		return retStr;
	}

	void NetTest::ClientFun(NetTest *test)
	{
		if (test->m_client_socket == SOCKET_ERROR)
		{
			cout << "the client socket is error" << endl;
			return;
		}

		const int buffsize = 8 * 1024;
		SOCKET s = test->m_client_socket;
		test->m_client_socket = SOCKET_ERROR;
		int recvbuffsize = buffsize;
		int sendbuffsize = buffsize;
		::setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char *)&recvbuffsize, sizeof(int));
		::setsockopt(s, SOL_SOCKET, SO_SNDBUF, (const char *)&sendbuffsize, sizeof(int));

		Byte recvbuff[buffsize];
		Byte sendbuff[buffsize];
		int recvsize = 0;
		int sendsize = 0;
		fd_set readfds;
		fd_set writefds;
		fd_set exceptfds;

		cout << "start client recv/send loop..." << endl;
		while (test->m_running)
		{
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
			FD_SET(s, &readfds);
			FD_SET(s, &writefds);
			FD_SET(s, &exceptfds);

			bool needsleep = true;
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
					if (recvsize < buffsize)
					{
						int len = ::recv(s, (char*)recvbuff + recvsize, buffsize - recvsize, 0);
						if (len > 0)
						{
							recvsize += len;
							cout << "recv data size:" << len << endl;
							recvsize = 0;

							NetPackageHeader recvhead;
							int i = 0;
							i= recvhead.Unpack(recvbuff, i);
							string text = DataUtil::ReadString(recvbuff, i, &i);
							cout << "text:" << UTF8_To_string(text) << endl;
							needsleep = false;			//有数据接收就不休息了

							//准备回复数据							
							Byte recvdata[1024];
							int index = 0;

							int result = 4;
							index = DataUtil::WriteInt32(recvdata, index, result);
							string ret =  text + "吼吼吼吼";
							index = DataUtil::WriteString(recvdata, index, ret);
							Int64 freetime = 0;
							index = DataUtil::WriteInt64(recvdata, index, freetime);

							//回复的数据头
							NetPackageHeader sendhead(recvhead);
							sendhead.Command = 1050;
							sendhead.BodySize = index;

							sendsize = sendhead.Pack(sendbuff + sendsize, sendsize);
							::memcpy_s(sendbuff + sendsize, buffsize - sendsize, recvdata, index);
							sendsize += index;
						}
						else if (len == 0)
						{
							//客户端关闭了连接
							cout << "remote close the connect..." << endl;
							break;
						}
						else
						{
							cout << "recv socket err:" << WSAGetLastError() << endl;
							break;
						}
					}
					else
					{
						//缓冲区满了 理论上不应该出现
					}
				}
				if (FD_ISSET(s, &writefds) != 0)
				{
					//看有数据是否需要发送
					if (sendsize > 0)
					{
						int len = ::send(s, (char*)sendbuff, sendsize, 0);
						if (len > 0)
						{
							sendsize -= len;
							::memcpy_s(sendbuff, buffsize, sendbuff + len, sendsize);
							needsleep = false;			//有数据发送就不休息了
						}
						else if (len == 0)
						{
							//客户端关闭了连接
							cout << "remote close the connect..." << endl;
							break;
						}
						else
						{
							cout << "send socket err:" << WSAGetLastError() << endl;
							break;
						}
					}
				}
			}
			else if (ret == 0)
			{
				//没有消息
			}
			else if (ret == SOCKET_ERROR)
			{
				cout << "select socket err:" << WSAGetLastError() << endl;
				break;
			}

			//统一等待100毫秒
			if (needsleep)
			{
				::Sleep(100);
			}			
		}

		//监听结束
		::closesocket(s);
	}
}