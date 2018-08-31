/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetSocketThread.h
* Summary: 管理一组Socket的线程。
*
* Author: XuXiang
* Date: 2018-08-13 22:48
*******************************************************/

#ifndef __NetSocketThread_h__
#define __NetSocketThread_h__

#include <WinSock2.h>
#include <map>
#include <thread>
using namespace std;

namespace XX004
{
	namespace Net
	{
		//用于对Socket的封装
		class NetSocketWrap
		{
			friend class NetSocketThread;
		public:
			NetSocketWrap();
			virtual ~NetSocketWrap();

			//判断是否需要读数据
			virtual bool IsNeedRead() { return true; }

			//判断是否需要写数据
			virtual bool IsNeedWrite() { return true; }

			//获取Sokcet ret:Socket
			inline SOCKET GetSocket()const { return m_Socket; }

		private:

			//设置Socket
			inline void SetSocket(SOCKET s) { m_Socket = s; }

			SOCKET m_Socket;
		};

		//线程类
		class NetSocketThread
		{
			typedef map<SOCKET, NetSocketWrap*> SocketMap;

		public:
			NetSocketThread();
			virtual ~NetSocketThread();

			//Socket可以读取数据了
			virtual int OnSocketRead(NetSocketWrap *wrap) = 0;

			//Socket可以写入数据了
			virtual int OnSocketWrite(NetSocketWrap *wrap) = 0;

			//Socket关闭了
			virtual void OnSocketClose(NetSocketWrap *wrap) = 0;

			//开始线程
			void Start();

			//结束线程
			void Stop();

			//等待线程结束
			void Join();

		private:

			void ThreadProcess();

			//Socket的集合
			SocketMap m_Sockets;

			//是否在运行中
			bool m_Running;

			//是否要停止线程
			bool m_Stop;

			//Socket处理线程
			thread m_Thread;
		};
	}
}


#endif	//__NetSocketThread_h__
