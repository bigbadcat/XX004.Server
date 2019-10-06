/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: StorageManager.h
* Summary: 存储模块管理。
*
* Author: XuXiang
* Date: 2019-10-05 10:53
*******************************************************/

#ifndef __StorageManager_h__
#define __StorageManager_h__

#include "NetManagerBase.h"
#include "MySQL/MySQLWrap.h"
#include <thread>
#include <string>
#include <map>
using namespace std;
using namespace XX004::Net;

namespace XX004
{
#define DB_REGISTER(mgr, id, fun) mgr->RegisterMessageCallBack(id, fun)

	//创建独立线程来进行数据库存储
	//数据通信基于网络
	class StorageManager
	{
	public:
		StorageManager();
		virtual ~StorageManager();

		//启动
		void Start();

		//停止
		void Stop();

		//注册存储处理函数
		void RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call);

		//注销存储处理函数
		void UnregisterMessageCallBack(Int32 cmd);

		//注销所有存储处理函数
		void UnregisterAllCallBack();

		//发送数据请求
		void Request(Int64 rid, int command, NetMessage *msg);

		//获取数据库
		inline MySQLWrap* GetMySQL() { return &m_MySQL; }

	private:
		//分发请求
		void Dispatch();

		//分发消息
		void OnDispatch(NetDataItem *item);

		//创建一个数据项
		NetDataItem* GetNetDataItem();

		//回收数据对象
		void CacheNetDataItem(NetDataItem *item);

		//线程过程
		void ThreadProcess();


		//是否运行
		bool m_IsRunning;

		//网络处理线程
		thread m_Thread;

		//消息集合
		MessageCallBackMap m_CallBack;

		//请求的数据队列
		NetDataItemLockQueue m_RequestQueue;

		//缓存队列
		NetDataItemLockQueue m_CacheQueue;

		//MySQL
		MySQLWrap m_MySQL;

	};
}

#endif
