/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnectionSet.h
* Summary: 网络通信模块服务端连接集合。
*
* Author: XuXiang
* Date: 2018-09-12 23:23
*******************************************************/

#ifndef __NetConnectionSet_h__
#define __NetConnectionSet_h__

#include "NetConnection.h"
#include "NetSocketThread.h"

namespace XX004
{
	namespace Net
	{
		class NetConnection;
		class NetConnectionManager;

		//网络连接线程，管理一组连接
		class NetConnectionSet
		{
			typedef std::map<SOCKET, NetConnection*> ConnectionMap;

		public:
			//构造析构函数
			NetConnectionSet();
			virtual ~NetConnectionSet();


			//Socket可以读取数据了
			int OnSocketRead(SOCKET s);

			//Socket可以写入数据了
			int OnSocketWrite(SOCKET s);

			//Socket关闭
			void OnSocketClose(SOCKET s);

			//选择Socket处理
			void SelectSocket();

			//添加连接
			NetConnection* AddConnection(SOCKET s);

			//获取连接
			NetConnection* GetConnection(SOCKET s);

			//关闭连接
			void CloseConnection(NetConnection *con);
			
			//获取管理者
			inline NetConnectionManager* GetManager()const { return m_pManager; }

			//设置管理者
			inline void SetManager(NetConnectionManager* manager) { m_pManager = manager; }


			//一个线程管理的最大连接数
			static const std::size_t MAX_CONNECTION_NUMBER = 64;

		private:

			//所属的管理者
			NetConnectionManager* m_pManager;

			//连接集合
			ConnectionMap m_Connections;
		};
	}
}

#endif	//__NetConnectionSet_h__