#ifndef __NetListener_h__
#define __NetListener_h__

#include "NetSocketThread.h"

namespace XX004
{
	namespace Net
	{
		//服务端的网络连接监听者。
		class NetListener : public NetSocketThread
		{
		public:
			//构造析构函数
			NetListener();
			virtual ~NetListener();

			//启动监听。
			//ipaddress:IP地址。
			//port:端口号。
			void Start(string ipaddress, int port);

			//Socket可以读取数据了
			virtual int OnSocketRead(NetSocketWrap *wrap);

			//Socket可以写入数据了
			virtual int OnSocketWrite(NetSocketWrap *wrap);

			//Socket关闭了，此时wrap的Socket成员已经被重置成SOCKET_ERROR
			virtual void OnSocketClose(NetSocketWrap *wrap);

		protected:

			//线程开始
			virtual void OnBegin();

			//线程结束
			virtual void OnEnd();

		private:
			//IP地址。
			string m_IPAddress;

			//端口号。
			int m_Port;
		};
	}
}

#endif	//__NetListener_h__