#ifndef __NetConnectionThread_h__
#define __NetConnectionThread_h__

#include "NetConnection.h"
#include "NetSocketThread.h"

namespace XX004
{
	namespace Net
	{
		class NetConnection;
		class NetConnectionManager;

		//网络连接线程，管理一组连接
		class NetConnectionThread : public NetSocketThread
		{
			typedef std::map<SOCKET, NetConnection*> ConnectionMap;

		public:
			//构造析构函数
			NetConnectionThread();
			virtual ~NetConnectionThread();

			//Socket可以读取数据了
			virtual int OnSocketRead(NetSocketWrap *wrap);

			//Socket可以写入数据了
			virtual int OnSocketWrite(NetSocketWrap *wrap);

			//Socket关闭了，此时wrap的Socket成员已经被重置成SOCKET_ERROR
			virtual void OnSocketClose(NetSocketWrap *wrap);

			//添加连接
			NetConnection* AddConnection(SOCKET s);

			//获取连接
			NetConnection* GetConnection(SOCKET s);
			
			//获取管理者
			inline NetConnectionManager* GetManager()const { return m_pManager; }

			//设置管理者
			inline void SetManager(NetConnectionManager* manager) { m_pManager = manager; }


			//一个线程管理的最大连接数
			static const std::size_t MAX_CONNECTION_NUMBER = 64;

		protected:

			//线程开始
			virtual void OnBegin();

			//线程结束
			virtual void OnEnd();

		private:

			//所属的管理者
			NetConnectionManager* m_pManager;

			//连接集合
			ConnectionMap m_Connections;
		};
	}
}

#endif	//__NetConnectionThread_h__