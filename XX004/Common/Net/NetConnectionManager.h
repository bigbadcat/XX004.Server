#ifndef __NetConnectionManager_h__
#define __NetConnectionManager_h__

#include <WinSock2.h>
#include <vector>
#include <map>
#include "NetConnection.h"

namespace XX004
{
	namespace Net
	{
		class NetServer;
		class NetConnectionThread;

		//网络连接管理		
		class NetConnectionManager
		{
			typedef std::vector<NetConnectionThread*> ConnectionVector;
			typedef std::map<RemoteKey, NetConnection*> ConnectionMap;

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

			//获取服务端
			inline NetServer* GetServer()const { return m_pServer; }

			//添加连接
			NetConnection* AddConnection(SOCKET s);

			//移除连接
			void RemoveConnection(NetConnection* con);

			//获取连接
			NetConnection* GetConnection(SOCKET s);

			//设置远端标识
			void SetRemote(NetConnection* con, const RemoteKey& key);

			//获取连接
			NetConnection* GetConnection(const RemoteKey& key);

		private:
			//网络服务端
			NetServer *m_pServer;

			//连接线程
			ConnectionVector m_ConnectionThreads;

			//远端标识到连接映射
			ConnectionMap m_RemoteToConnection;
		};
	}
}

#endif	//__NetConnectionManager_h__