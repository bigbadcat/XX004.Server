/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetSocketThread.h
* Summary: ����һ��Socket���̡߳�
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
		//���ڶ�Socket�ķ�װ
		class NetSocketWrap
		{
			friend class NetSocketThread;
		public:
			NetSocketWrap();
			virtual ~NetSocketWrap();

			//�ж��Ƿ���Ҫ������
			virtual bool IsNeedRead() { return true; }

			//�ж��Ƿ���Ҫд����
			virtual bool IsNeedWrite() { return true; }

			//��ȡSokcet ret:Socket
			inline SOCKET GetSocket()const { return m_Socket; }

		private:

			//����Socket
			inline void SetSocket(SOCKET s) { m_Socket = s; }

			SOCKET m_Socket;
		};

		//�߳���
		class NetSocketThread
		{
			typedef map<SOCKET, NetSocketWrap*> SocketMap;

		public:
			NetSocketThread();
			virtual ~NetSocketThread();

			//Socket���Զ�ȡ������
			virtual int OnSocketRead(NetSocketWrap *wrap) = 0;

			//Socket����д��������
			virtual int OnSocketWrite(NetSocketWrap *wrap) = 0;

			//Socket�ر���
			virtual void OnSocketClose(NetSocketWrap *wrap) = 0;

			//��ʼ�߳�
			void Start();

			//�����߳�
			void Stop();

			//�ȴ��߳̽���
			void Join();

		private:

			void ThreadProcess();

			//Socket�ļ���
			SocketMap m_Sockets;

			//�Ƿ���������
			bool m_Running;

			//�Ƿ�Ҫֹͣ�߳�
			bool m_Stop;

			//Socket�����߳�
			thread m_Thread;
		};
	}
}


#endif	//__NetSocketThread_h__
