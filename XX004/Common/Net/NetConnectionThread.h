#ifndef __NetConnectionThread_h__
#define __NetConnectionThread_h__

#include "NetSocketThread.h"

namespace XX004
{
	namespace Net
	{
		//网络连接对象。
		class NetConnection;
		class NetConnectionThread : public NetSocketThread
		{
		public:
			//构造析构函数
			NetConnectionThread();
			virtual ~NetConnectionThread();

			//添加连接
			NetConnection* AddConnection(SOCKET s);
		};
	}
}

#endif	//__NetConnectionThread_h__