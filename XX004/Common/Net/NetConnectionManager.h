﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnectionManager.h
* Summary: 网络通信模块服务端连接管理器。
*
* Author: XuXiang
* Date: 2018-07-30 23:07
*******************************************************/

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
		class NetConnectionSet;

		//网络连接管理，通过管理多个连接线程实现
		class NetConnectionManager
		{
			typedef std::vector<NetConnectionSet*> ConnectionVector;

		public:
			//构造析构函数
			NetConnectionManager();
			virtual ~NetConnectionManager();

			//初始化
			void Init();
			
			//释放
			void Release();

			//选择Socket处理
			void SelectSocket();

			//设置服务端
			inline void SetServer(NetServer *p) { m_pServer = p; }

			//获取服务端
			inline NetServer* GetServer()const { return m_pServer; }

			//添加连接
			NetConnection* AddConnection(SOCKET s);

			//移除连接
			void OnRemoveConnection(NetConnection* con);

			//获取连接
			NetConnection* GetConnection(SOCKET s);

			////设置远端标识
			//void SetRemote(NetConnection* con, const RemoteKey& key);

			////获取连接
			//NetConnection* GetConnection(const RemoteKey& key);

			//有连接接收到数据包了
			void OnRecvPackage(NetConnection *con);

		private:
			//网络服务端
			NetServer *m_pServer;

			//连接线程
			ConnectionVector m_ConnectionSets;

		};
	}
}

#endif	//__NetConnectionManager_h__