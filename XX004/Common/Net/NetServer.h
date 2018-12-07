/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetServer.h
* Summary: 网络通信模块服务端。
*
* Author: XuXiang
* Date: 2018-07-30 22:49
*******************************************************/

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
		class NetConnection;

		//网络处理接口
		class INetProcesser
		{
		public:
			//有连接建立
			virtual void OnConnected(NetConnection *connection) = 0;

			//有链接断开
			virtual void OnDisconnected(NetConnection *connection) = 0;

			//有数据接收
			virtual void OnRecvData(NetConnection *connection, const NetPackageHeader& header, Byte *buffer) = 0;
		};

		//服务端网络管理。		
		class NetServer
		{
			friend class NetListener;
			friend class NetConnectionManager;

		public:
			//构造析构函数
			NetServer();
			~NetServer();

			//启动网络模块。port:监听的端口号。
			void Start(int port);

			//停止服务
			void Stop();

			//选择Socket处理
			void SelectSocket();

			//获取连接
			NetConnection* GetConnection(UInt64 uid);

			//获取连接
			NetConnection* GetConnection(const RemoteKey &key);

			//设置连接标识
			void SetRemote(UInt64 uid, const RemoteKey &key);

			//关闭连接
			void CloseConnection(NetConnection* con);

			//设置网络处理者，处理者生命周期不由NetServer管理
			inline void SetProcesser(INetProcesser *p) { m_pProcesser = p; }

			//获取网络处理者
			inline INetProcesser* GetProcesser()const { return m_pProcesser; }

		private:
			//有新的连接。
			void OnConnect(SOCKET s);

			//有连接断开。
			void OnDisconnect(NetConnection* con);

			//有连接接收到数据包
			void OnRecvData(NetConnection* con);
			
			//监听者
			NetListener *m_pListener;

			//连接管理
			NetConnectionManager *m_pConnectionManager;

			//网络处理
			INetProcesser* m_pProcesser;
		};
	}
}

#endif	//__NetServer_h__