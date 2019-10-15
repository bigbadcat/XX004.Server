/*******************************************************
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
#include "NetDefine.h"

namespace XX004
{
	namespace Net
	{
		class NetServer;
		class NetListener;
		class NetConnection;

		//网络连接管理，通过管理多个连接线程实现
		class NetConnectionManager
		{
			typedef std::map<SOCKET, NetConnection*> NetConnectionMap;
			typedef std::map<UInt64, NetConnection*> UIDToConnectionMap;
			typedef std::map<RemoteKey, NetConnection*> RemoteKeyToConnectionMap;

		public:
			//构造析构函数
			NetConnectionManager();
			virtual ~NetConnectionManager();

			//初始化
			void Init(int port);
			
			//释放
			void Release();

			//选择Socket处理
			void SelectSocket(int msec);

			//设置服务端
			inline void SetServer(NetServer *p) { m_pServer = p; }

			//获取服务端
			inline NetServer* GetServer()const { return m_pServer; }

			//添加连接
			void AddConnection(SOCKET s);

			//移除并关闭连接
			void RemoveConnection(NetConnection* con);

			//获取连接
			NetConnection* GetConnection(UInt64 uid)const;

			//获取连接
			NetConnection* GetConnection(const RemoteKey &key)const;

			//设置连接标识
			void SetRemote(UInt64 uid, const RemoteKey &key);

			//有连接接收到数据包了
			void OnRecvPackage(NetConnection *con);

		private:

			//获取连接
			NetConnection* GetConnectionFromSocket(SOCKET s)const;

			//Socket可以读取数据了
			int OnSocketRead(SOCKET s);

			//Socket可以写入数据了
			int OnSocketWrite(SOCKET s);

			//Socket关闭
			void OnSocketClose(SOCKET s);

			//网络服务端
			NetServer *m_pServer;

			//监听者
			NetListener *m_pListener;

			//连接集合
			NetConnectionMap m_Connections;

			//唯一标识到连接(辅助查询)
			UIDToConnectionMap m_UIDToConnection;

			//远端标识到连接(辅助查找)
			RemoteKeyToConnectionMap m_RemoteKeyToConnection;
		};
	}
}

#endif