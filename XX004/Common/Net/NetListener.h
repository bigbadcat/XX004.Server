#ifndef __NetListener_h__
#define __NetListener_h__

#include "NetSocketThread.h"

namespace XX004
{
	namespace Net
	{
		class NetServer;

		class NetListenerSocketWrap : public NetSocketWrap
		{
			//判断是否需要写数据
			virtual bool IsNeedWrite() { return false; }
		};

		//服务端的网络连接监听者		
		class NetListener : public NetSocketThread
		{
		public:
			//构造析构函数
			NetListener();
			virtual ~NetListener();

			//启动监听。
			//ipaddress:IP地址。
			//port:端口号。
			void Start(const string &ipaddress, int port);

			//Socket可以读取数据了
			virtual int OnSocketRead(NetSocketWrap *wrap);

			//Socket可以写入数据了
			virtual int OnSocketWrite(NetSocketWrap *wrap);

			//Socket关闭了，此时wrap的Socket成员已经被重置成SOCKET_ERROR
			virtual void OnSocketClose(NetSocketWrap *wrap);

			//设置服务端
			inline void SetServer(NetServer *p) { m_pServer = p; }

		protected:

			//线程开始
			virtual void OnBegin();

			//线程结束
			virtual void OnEnd();

		private:

			//创建监听Socket
			SOCKET CreateListenSocket();

			//IP地址。
			string m_IPAddress;

			//端口号。
			int m_Port;

			//监听Socket
			NetListenerSocketWrap m_ListenSocket;

			//网络服务端
			NetServer *m_pServer;
		};
	}
}

#endif	//__NetListener_h__