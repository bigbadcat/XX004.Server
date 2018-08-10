#ifndef __NetServer_h__
#define __NetServer_h__

#include <iostream>
#include <string>

using namespace std;

namespace XX004
{
	namespace Net
	{
		//网络处理接口
		class INetProcesser
		{
		public:

		};

		//服务端网络管理。
		class NetServer
		{
		public:
			//启动网络模块。 ipaddress:IP地址。 port:端口号。
			void Start(string ipaddress, int port);
		};
	}
}

#endif	//__NetServer_h__