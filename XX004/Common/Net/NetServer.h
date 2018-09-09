﻿#ifndef __NetServer_h__
#define __NetServer_h__

#include <iostream>
#include <string>
#include "NetConnection.h"
#include"../Macro.h"

using namespace std;

namespace XX004
{
	namespace Net
	{
		//网络处理接口
		class INetProcesser
		{
		public:
			virtual void OnConnected(NetConnection *connection) = 0;
			virtual void OnDisconnected(NetConnection *connection) = 0;
			virtual void OnRecvData(NetConnection *connection, Int16 cmd, Byte *buffer, int len) = 0;
		};

		//服务端网络管理。
		class NetServer
		{
		public:
			//启动网络模块。 ipaddress:IP地址。 port:端口号。
			void Start(string ipaddress, int port);

			//停止服务
			void Stop();
			
		};
	}
}

#endif	//__NetServer_h__