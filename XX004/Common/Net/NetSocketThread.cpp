/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetSocketThread.cpp
* Summary: ����һ��Socket���̡߳�
*
* Author: XuXiang
* Date: 2018-08-13 22:48
*******************************************************/

#include "NetSocketThread.h"
#include <vector>
#include <iostream>

namespace XX004
{
	namespace Net
	{
		NetSocketWrap::NetSocketWrap() : m_Socket(SOCKET_ERROR)
		{

		}

		NetSocketWrap::~NetSocketWrap()
		{

		}

		NetSocketThread::NetSocketThread() : m_Running(false), m_Stop(false)
		{

		}

		NetSocketThread::~NetSocketThread()
		{
			if (m_Thread.joinable())
			{
				m_Thread.join();
			}
		}

		void NetSocketThread::ThreadProcess()
		{
			fd_set readfds;
			fd_set writefds;
			fd_set exceptfds;
			vector<SOCKET> needremove;
			while (!m_Stop)
			{
				FD_ZERO(&readfds);
				FD_ZERO(&writefds);
				FD_ZERO(&exceptfds);				
				
				//��ʼ��FD����
				for (SocketMap::iterator itr = m_Sockets.begin(); itr != m_Sockets.end(); ++itr)
				{
					SOCKET s = itr->second->GetSocket();
					if (itr->second->IsNeedRead())
					{
						FD_SET(s, &readfds);
					}
					if (itr->second->IsNeedWrite())
					{
						FD_SET(s, &writefds);
					}
					FD_SET(s, &exceptfds);
				}

				//����
				timeval timeout;
				timeout.tv_sec = 0;
				timeout.tv_usec = 0;
				int ret = ::select(0, &readfds, &writefds, &exceptfds, &timeout);		//windows��nfds�������ã��ɴ���0
				if (ret > 0)
				{
					for (SocketMap::iterator itr = m_Sockets.begin(); itr != m_Sockets.end(); ++itr)
					{
						SOCKET s = itr->second->GetSocket();
						needremove.clear();

						//���ж��Ƿ����쳣
						if (FD_ISSET(s, &exceptfds) != 0)
						{
							needremove.push_back(s);
							continue;
						}
						if (FD_ISSET(s, &readfds) != 0)
						{
							if (OnSocketRead(itr->second) != 0)
							{
								needremove.push_back(s);
								continue;
							}
						}
						if (FD_ISSET(s, &writefds) != 0)
						{
							if (OnSocketWrite(itr->second) != 0)
							{
								needremove.push_back(s);
								continue;
							}
						}
					}

					//�Ƴ������SOKECT
					for (vector<SOCKET>::iterator itr = needremove.begin(); itr != needremove.end(); ++itr)
					{
						NetSocketWrap *wrap = m_Sockets[*itr];
						m_Sockets.erase(wrap->GetSocket());
						::closesocket(wrap->GetSocket());
						wrap->SetSocket(SOCKET_ERROR);
						OnSocketClose(wrap);
					}
				}
				else if (ret == 0)
				{
					//û����Ϣ �ٵ�100�������
					::Sleep(100);
				}
				else if (ret == SOCKET_ERROR)
				{
					cout << "select socket err:" << WSAGetLastError() << endl;
					break;
				}
			}

			m_Stop = false;
			m_Running = false;
		}

		void NetSocketThread::Start()
		{
			if (m_Running)
			{
				return;
			}

			m_Running = true;
			m_Stop = false;
			m_Thread = thread([](NetSocketThread *t){t->~NetSocketThread(); }, this);
		}

		void NetSocketThread::Stop()
		{
			if (!m_Running || m_Stop)
			{
				return;
			}
			m_Stop = true;
		}

		void NetSocketThread::Join()
		{
			Stop();
			if (m_Thread.joinable())
			{
				m_Thread.join();
			}
		}
	}
}