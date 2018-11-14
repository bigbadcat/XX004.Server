/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetSocketThread.cpp
* Summary: 管理一组Socket的线程。
*
* Author: XuXiang
* Date: 2018-08-13 22:48
*******************************************************/

#include "NetSocketThread.h"
#include <vector>
#include <iostream>
#include <assert.h>
#include "../Macro.h"

namespace XX004
{
	namespace Net
	{
		NetSocketWrap::NetSocketWrap() : m_Socket(SOCKET_ERROR)
		{
		}

		NetSocketWrap::NetSocketWrap(SOCKET s) : m_Socket(s)
		{
		}

		NetSocketWrap::~NetSocketWrap()
		{
		}

		void NetSocketWrap::SetSocket(SOCKET s)
		{
			m_Socket = s;
		}


		NetSocketThread::SocketOperate::SocketOperate(NetSocketWrap *wrap) : OP(OP_ADD), Wrap(wrap), S(SOCKET_ERROR)
		{

		}

		NetSocketThread::SocketOperate::SocketOperate(SOCKET s) : OP(OP_REMOVE), Wrap(NULL), S(s)
		{
		}

		NetSocketThread::NetSocketThread() : m_Running(false), m_Stop(false)
		{
		}

		NetSocketThread::~NetSocketThread()
		{
			Join();
			for (SocketMap::iterator itr = m_Sockets.begin(); itr != m_Sockets.end(); ++itr)
			{
				::closesocket(itr->first);
			}
			m_Sockets.clear();
			while (m_Operates.size() > 0)
			{
				SocketOperate operate = m_Operates.front();
				m_Operates.pop();
				::closesocket(operate.Wrap->GetSocket());
			}
		}

		void NetSocketThread::ThreadProcess()
		{
			OnBegin();
			fd_set readfds;
			fd_set writefds;
			fd_set exceptfds;
			vector<SOCKET> needremove;
			while (!m_Stop)
			{
				//处理Socket操作
				HandleOperates();
				if (m_Sockets.size() <= 0)
				{
					::Sleep(100);
					continue;
				}

				//初始化FD集合
				FD_ZERO(&readfds);
				FD_ZERO(&writefds);
				FD_ZERO(&exceptfds);
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

				//处理
				timeval timeout;
				timeout.tv_sec = 0;
				timeout.tv_usec = 0;
				int ret = ::select(0, &readfds, &writefds, &exceptfds, &timeout);		//windows下nfds参数无用，可传入0
				if (ret > 0)
				{
					needremove.clear();
					for (SocketMap::iterator itr = m_Sockets.begin(); itr != m_Sockets.end(); ++itr)
					{
						SOCKET s = itr->second->GetSocket();

						//先判断是否有异常
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

					//移除错误的SOKECT
					for (vector<SOCKET>::iterator itr = needremove.begin(); itr != needremove.end(); ++itr)
					{
						NetSocketWrap *wrap = m_Sockets[*itr];
						m_Sockets.erase(wrap->GetSocket());
						::closesocket(wrap->GetSocket());						
						OnSocketClose(wrap);
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

			m_Stop = false;
			m_Running = false;
			OnEnd();
		}

		void NetSocketThread::HandleOperates()
		{
			m_OPMutex.lock();
			
			while (m_Operates.size() > 0)
			{
				SocketOperate operate = m_Operates.front();
				m_Operates.pop();
				if (operate.OP == SocketOperate::OP_ADD)
				{
					assert(operate.Wrap != NULL);
					SocketMap::iterator itr = m_Sockets.find(operate.Wrap->GetSocket());
					assert(itr == m_Sockets.end());
					m_Sockets.insert(SocketMap::value_type(operate.Wrap->GetSocket(), operate.Wrap));
				}
				else if (operate.OP == SocketOperate::OP_REMOVE)
				{
					assert(operate.S != SOCKET_ERROR);
					SocketMap::iterator itr = m_Sockets.find(operate.S);
					if (itr != m_Sockets.end())
					{
						NetSocketWrap *wrap = itr->second;
						m_Sockets.erase(itr);
						::closesocket(wrap->GetSocket());
						OnSocketClose(wrap);				
					}
				}
				else
				{
					assert(false);
				}
			}

			m_OPMutex.unlock();
		}

		void NetSocketThread::Start()
		{
			if (m_Running)
			{
				return;
			}

			m_Running = true;
			m_Stop = false;
			m_Thread = thread([](NetSocketThread *t){t->ThreadProcess(); }, this);
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
			JoinThread(m_Thread);
		}

		void NetSocketThread::AddSocket(NetSocketWrap *wrap)
		{
			m_OPMutex.lock();
			m_Operates.push(SocketOperate(wrap));
			m_OPMutex.unlock();
		}

		void NetSocketThread::RemoveSocket(SOCKET s)
		{
			m_OPMutex.lock();
			m_Operates.push(SocketOperate(s));
			m_OPMutex.unlock();
		}
	}
}