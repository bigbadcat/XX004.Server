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
#include <string>
#include "Net/NetServer.h"
#include "Net/NetMessage.h"
#include "Net/NetConnection.h"
#include "Net/NetInternalConnection.h"
#include "Core/semaphore.h"
#include "Util/LockQueue.h"
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
	typedef std::function<void(NetConnection*, int, Byte*, int)> NetMessageInternalCallBack;
	typedef std::map<Int32, NetMessageInternalCallBack> NetMessageInternalCallBackMap;

	typedef std::queue<NetDataItem*> NetDataItemQueue;
	typedef LockQueue<NetDataItem> NetDataItemLockQueue;	

	typedef std::map<int, NetInternalConnection*> InternalConnectionMap;	//内部连接集合

	std::ostream & operator<<(std::ostream &out, RemoteKey &key);
	
#define NET_REGISTER(mgr, id, fun) mgr->RegisterMessageCallBack(id, [this](NetDataItem *item){this->fun(item); })

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
		void Start(int port);

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
		void Send(UInt64 uid, int command, NetMessage *msg);

		//发送数据
		void Send(const RemoteKey& key, int command, Byte *buffer, int len);

		//发送数据
		void Send(const RemoteKey& key, int command, NetMessage *msg);

		//关闭连接
		void Close(UInt64 uid);

		//关闭连接
		void Close(const RemoteKey& key);

	protected:
		//添加接收到的数据
		virtual void OnAddRecvData(NetDataItem *item);

		//创建内部连接信息
		virtual void OnCreateInternalInfo(std::vector<int> &infos);

	private:

		//创建一个数据项
		NetDataItem* GetNetDataItem();

		//回收数据对象
		void CacheNetDataItem(NetDataItem *item);

		//分发消息
		void OnDispatch(NetDataItem *item);

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

		//更新内部连接
		void UpdateInternalConnection();

		//身份标识处理
		void OnMsgRemoteIdentify(NetConnection *connection, Int32 cmd, Byte *buffer, int len);


		//是否运行
		bool m_IsRunning;

		//网络处理线程
		thread m_Thread;

		//端口
		int m_Port;

		//网络服务端模块
		NetServer m_Server;

		//启动信号
		xxstd::semaphore m_InitSemaphore;

		//建立连接回调
		NetMessageCallBack m_OnConnectCallBack;

		//断开连接回调
		NetMessageCallBack m_OnDisconnectCallBack;

		//消息集合
		MessageCallBackMap m_CallBack;

		//内部消息处理
		NetMessageInternalCallBackMap m_InternalCallBack;

		//要发送的数据队列
		NetDataItemLockQueue m_SendQueue;

		//接收到的数据队列
		NetDataItemLockQueue m_RecvQueue;

		//缓存队列
		NetDataItemLockQueue m_CacheQueue;

		//内部连接
		InternalConnectionMap m_InternalConnections;
	};
}

#endif	//__NetManagerBase_h__