/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: StorageManager.cpp
* Summary: 存储模块管理。
*
* Author: XuXiang
* Date: 2019-10-05 10:53
*******************************************************/

#include "StorageManager.h"
#include "../MySQL/MySQLWrap.h"
#include "MainBase.h"
#include "../Macro.h"
#include "StartSetting.h"
#include <assert.h>
#include <iostream>

namespace XX004
{
	StorageManager::StorageManager() : m_IsRunning(false), m_MySQL(new MySQLWrap())
	{

	}

	StorageManager::~StorageManager()
	{
		SAFE_DELETE(m_MySQL);
	}

	void StorageManager::Start(int type, int sid)
	{
		const DataBaseSetting* db = StartSetting::GetInstance()->GetDataBase();
		m_IsRunning = true;
		m_MySQL->Init(db->GetHost().c_str(), db->GetUser().c_str(), db->GetPassword().c_str(), db->GetName().c_str(), db->GetPort());
		StartSetting::GetInstance()->LoadServerInfo(type, sid, m_MySQL);
		m_Thread = thread([](StorageManager *t){t->ThreadProcess(); }, this);
	}

	void StorageManager::Stop()
	{
		m_IsRunning = false;
		JoinThread(m_Thread);
	}

	void StorageManager::RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call)
	{
		m_CallBack[cmd] = call;
	}

	void StorageManager::UnregisterMessageCallBack(Int32 cmd)
	{
		m_CallBack.erase(cmd);
	}

	void StorageManager::UnregisterAllCallBack()
	{
		m_CallBack.clear();
	}

	void StorageManager::Request(Int64 rid, int command, NetMessage *msg)
	{
		int t = MainBase::GetCurMain()->GetType();
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DATA;
		item->key = RemoteKey(t, rid);
		item->cmd = command;
		item->len = msg->Pack(item->data, 0);
		m_RequestQueue.Push(item);
	}

	void StorageManager::Dispatch()
	{
		//先进行非线程安全的只读判断
		if (m_RequestQueue.GetSize() <= 0)
		{
			return;
		}

		//先锁定队列，将消息都拿出来，解锁队列
		static NetDataItemQueue temp_queue;
		m_RequestQueue.MoveAll(temp_queue);

		//分发消息
		while (temp_queue.size() > 0)
		{
			NetDataItem *item = temp_queue.front();
			temp_queue.pop();
			OnDispatch(item);
			CacheNetDataItem(item);
		}
	}

	void StorageManager::OnDispatch(NetDataItem *item)
	{
		MessageCallBackMap::iterator itr = m_CallBack.find(item->cmd);
		if (itr != m_CallBack.end())
		{
			(itr->second)(item);
		}
		else
		{
			cout << "Unkonw storage request " << item->cmd << endl;
		}
	}

	NetDataItem* StorageManager::GetNetDataItem()
	{
		//先从缓存队列中获取
		NetDataItem *item = m_CacheQueue.TryPop();
		if (item == NULL)
		{
			//没有再创建
			item = new NetDataItem();
		}
		item->Reset();
		return item;
	}


	void StorageManager::CacheNetDataItem(NetDataItem *item)
	{
		assert(item != NULL);
		m_CacheQueue.Push(item);
	}

	void StorageManager::ThreadProcess()
	{
		std::chrono::milliseconds dura(100);
		while (m_IsRunning)
		{
			Dispatch();
			std::this_thread::sleep_for(dura);
		}
	}
}