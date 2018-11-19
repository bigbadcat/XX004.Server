﻿/*******************************************************
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
#include "Core\semaphore.h"
using namespace XX004::Net;

namespace XX004
{
	//网络数据项
	struct NetDataItem
	{
		//连接标识
		Int64 sid;

		//协议号
		Int32 cmd;

		//消息数据内容
		Byte data[NET_PACKAGE_MAX_SIZE];

		//消息数据长度
		int len;

		//构造函数
		NetDataItem();

		//清空数据
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

		virtual void OnConnected(NetConnection *connection);
		virtual void OnDisconnected(NetConnection *connection);
		virtual void OnRecvData(NetConnection *connection, const NetPackageHeader& header, Byte *buffer);

		//注册网络消息处理函数
		void RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call);

		//注销网络消息处理函数
		void UnregisterMessageCallBack(Int32 cmd);

		//注销所有网络消息处理函数
		void UnregisterAllCallBack();

		//启动网络
		void Start();

		//停止网络
		void Stop();

		//等待网络
		void Join();

		//分发消息给Server
		void Dispatch();

		//发送数据
		void Send(Int64 sid, int command, Byte *buffer, int len);

	private:

		//创建一个数据项
		NetDataItem* GetNetDataItem();

		//回收数据对象
		void CacheNetDataItem(NetDataItem *item);

		//线程过程
		void ThreadProcess();

		//提交发送的数据给Net
		void OnPostSend();

		//处理Socket
		virtual void OnSocketSelect();

		//提交数据到Net
		virtual void OnPost(NetDataItem *item);

		
		//消息集合
		MessageCallBackMap m_CallBack;

		//要发送的数据队列
		NetDataItemQueue m_SendQueue;

		//发送队列锁
		std::mutex m_SendMutex;

		//接收到的数据队列
		NetDataItemQueue m_RecvQueue;

		//接收队列锁
		std::mutex m_RecvMutex;

		//缓存队列
		NetDataItemQueue m_CacheQueue;

		//缓存队列锁
		std::mutex m_CacheMutex;

		//是否运行
		bool m_IsRunning;

		//网络处理线程
		thread m_Thread;

		//网络服务端模块
		NetServer m_Server;

		//启动信号
		xxstd::semaphore m_InitSemaphore;

	};
}

#endif	//__NetManagerBase_h__