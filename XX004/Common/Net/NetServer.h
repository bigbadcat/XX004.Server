#ifndef __NetServer_h__
#define __NetServer_h__

#include <iostream>
#include <string>
#include <WinSock2.h>
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
			friend class NetListener;
			friend class NetConnectionManager;
		public:
			NetServer();
			~NetServer();

			//启动网络模块。 ipaddress:IP地址。 port:端口号。
			void Start(const string &ipaddress, int port);

			//停止服务
			void Stop();

		private:
			//有新的连接。
			void OnConnect(SOCKET s);

			//有连接断开。
			void OnDisconnect(SOCKET s);
			
			//监听者
			NetListener *m_pListener;

			//连接管理
			NetConnectionManager *m_pConnectionManager;
		};
	}
}

#endif	//__NetServer_h__