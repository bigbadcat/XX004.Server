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
#include <queue>
#include <thread>
#include <mutex>
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
			NetSocketWrap(SOCKET s);
			virtual ~NetSocketWrap();

			//判断是否需要读数据
			virtual bool IsNeedRead() { return true; }

			//判断是否需要写数据
			virtual bool IsNeedWrite() { return true; }

			//获取Sokcet ret:Socket
			inline SOCKET GetSocket()const { return m_Socket; }

			//设置Socket
			virtual void SetSocket(SOCKET s);

		private:
			
			SOCKET m_Socket;
		};

		//线程类
		class NetSocketThread
		{
			//Socket的操作信息
			struct SocketOperate
			{
				//添加操作
				static const int OP_ADD = 1;

				//移除操作
				static const int OP_REMOVE = 2;

				SocketOperate(NetSocketWrap *wrap);
				SocketOperate(SOCKET s);

				//操作
				int OP;

				//Socket封装，添加操作时使用
				NetSocketWrap *Wrap;

				//Socket，移除操作时使用
				SOCKET S;
			};

			typedef map<SOCKET, NetSocketWrap*> SocketMap;
			typedef queue<SocketOperate> SocketOperateQueue;

		public:
			//构造析构函数
			NetSocketThread();
			virtual ~NetSocketThread();

			//Socket可以读取数据了
			virtual int OnSocketRead(NetSocketWrap *wrap) = 0;

			//Socket可以写入数据了
			virtual int OnSocketWrite(NetSocketWrap *wrap) = 0;

			//Socket关闭了，此时wrap的Socket成员已经被重置成SOCKET_ERROR
			virtual void OnSocketClose(NetSocketWrap *wrap) = 0;

			//开始线程
			void Start();

			//结束线程
			void Stop();

			//等待线程结束
			void Join();

			//添加Socket
			void AddSocket(NetSocketWrap *wrap);

			//移除Socket，调用此函数后并不会立即移除SOCKET，由SocketThread自身循环移除，通过OnSocketClose通知
			void RemoveSocket(SOCKET s);

		protected:

			//线程开始
			virtual void OnBegin() = 0;

			//线程结束
			virtual void OnEnd() = 0;

		private:

			//线程过程
			void ThreadProcess();

			//处理操作
			void HandleOperates();

			//Socket的集合
			SocketMap m_Sockets;

			//是否在运行中
			bool m_Running;

			//是否要停止线程
			bool m_Stop;

			//Socket处理线程
			thread m_Thread;

			//操作队列
			SocketOperateQueue m_Operates;

			//操作队列临界区
			mutex m_OPMutex;
		};
	}
}


#endif	//__NetSocketThread_h__
