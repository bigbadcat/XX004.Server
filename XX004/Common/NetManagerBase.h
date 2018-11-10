/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerBase.h
* Summary: 网络管理的基类。
*
* Author: XuXiang
* Date: 2018-10-24 20:52
*******************************************************/

#ifndef __NetManagerBaseBase_h__
#define __NetManagerBaseBase_h__

#include <map>
#include <functional>
#include <queue>
#include "Net/NetServer.h"
#include "Net/NetMessage.h"
using namespace XX004::Net;

namespace XX004
{
	//网络数据项
	struct NetDataItem
	{
		//连接标识
		SOCKET s;

		//远端标识
		RemoteKey key;

		//协议号
		Int32 cmd;

		//消息数据内容
		Byte data[NET_PACKAGE_MAX_SIZE];

		//消息数据长度
		int len;

		void Reset();
	};
	
	//网络消息回调
	typedef std::function<void(NetDataItem*)> NetMessageCallBack;
	typedef std::map<Int32, NetMessageCallBack> MessageCallBackMap;
	typedef std::queue<NetDataItem*> NetDataItemQueue;
		
	//网络管理
	class NetManagerBase : public INetProcesser
	{
	public:
		NetManagerBase();
		virtual ~NetManagerBase();

		//启动网络 ipaddress:IP地址。 port:端口号。
		void Start(const string &ipaddress, int port);

		//停止网络
		void Stop();

		virtual void OnConnected(NetConnection *connection);
		virtual void OnDisconnected(NetConnection *connection);
		virtual void OnRecvData(NetConnection *connection, const NetPackageHeader& header, Byte *buffer);

		//注册网络消息处理函数
		void RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call);

		//注销网络消息处理函数
		void UnregisterMessageCallBack(Int32 cmd);

		//注销所有网络消息处理函数
		void UnregisterAllCallBack();

		//帧更新
		virtual void OnUpdate();

		//void Test(Int32 cmd);

	private:

		//创建一个数据项
		NetDataItem* CreateNetDataItem();

		//消息集合
		MessageCallBackMap m_CallBack;

		//网络服务端模块
		NetServer m_Server;

		//要发送的数据队列
		NetDataItemQueue m_SendQueue;

		//接收到的数据队列
		NetDataItemQueue m_RecvQueue;

		//发送锁
		std::mutex m_RecvMutex;

		//缓存队列
		NetDataItemQueue m_CacheQueue;
	};
}

#endif	//__NetManagerBase_h__