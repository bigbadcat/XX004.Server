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
#include "Core/semaphore.h"
using namespace XX004::Net;

namespace XX004
{
	//网络操作类型
	enum NetOperateType
	{
		//未知
		NOT_UNKNOW = 0,

		//建立连接
		NOT_CONNECT = 1,

		//断开连接
		NOT_DISCONNECT = 2,

		//网络数据
		NOT_DATA = 3,

		//更新数据
		NOT_UPDATE = 4,

		//关闭连接
		NOT_CLOSE = 5,
	};

	//网络数据项
	struct NetDataItem
	{
		//操作类型(NetOperateType)
		int op;

		//连接标识
		UInt64 uid;

		//远端标识
		RemoteKey key;

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


		//设置建立连接回调
		inline void SetOnConnectCallBack(NetMessageCallBack call) { m_OnConnectCallBack = call; }

		//获取建立连接回调
		inline NetMessageCallBack GetOnConnectCallBack()const { return m_OnConnectCallBack; }

		//设置断开连接回调
		inline void SetOnDisconnectCallBack(NetMessageCallBack call) { m_OnDisconnectCallBack = call; }

		//获取断开连接回调
		inline NetMessageCallBack GetOnDisconnectCallBack()const { return m_OnDisconnectCallBack; }

		//注册网络消息处理函数
		void RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call);

		//获取消息回调
		NetMessageCallBack GetMessageCallBack(Int32 cmd)const;

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

		//更新连接信息
		void Update(UInt64 uid, const RemoteKey& key);

		//发送数据
		void Send(UInt64 uid, int command, Byte *buffer, int len);

		//发送数据
		void Send(const RemoteKey& key, int command, Byte *buffer, int len);

		//关闭连接
		void Close(UInt64 uid);

		//关闭连接
		void Close(const RemoteKey& key);

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
		void OnPost(NetDataItem *item);

		//提交发送的网络数据
		virtual void OnPostData(NetDataItem *item);


		//建立连接回调
		NetMessageCallBack m_OnConnectCallBack;

		//断开连接回调
		NetMessageCallBack m_OnDisconnectCallBack;

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