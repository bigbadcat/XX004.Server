#ifndef __NetConnectionManager_h__
#define __NetConnectionManager_h__

#include <WinSock2.h>
#include <vector>

namespace XX004
{
	namespace Net
	{
		//网络连接管理。
		class NetServer;
		class NetConnection;
		class NetConnectionThread;
		class NetConnectionManager
		{
			typedef std::vector<NetConnectionThread*> ConnectionVector;
		public:
			//构造析构函数
			NetConnectionManager();
			virtual ~NetConnectionManager();

			//启动
			void Start();
			
			//停止
			void Stop();

			//等待完全结束
			void Join();

			//设置服务端
			inline void SetServer(NetServer *p) { m_pServer = p; }

			//添加连接
			NetConnection* AddConnection(SOCKET s);

		private:
			//网络服务端
			NetServer *m_pServer;

			//连接线程
			ConnectionVector m_ConnectionThreads;
		};
	}
}

#endif	//__NetConnectionManager_h__