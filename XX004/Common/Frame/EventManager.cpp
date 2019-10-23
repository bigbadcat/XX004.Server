﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: EventManager.cpp
* Summary: 事件系统。
*
* Author: XuXiang
* Date: 2019-10-22 17:10
*******************************************************/

#include "EventManager.h"
#include <assert.h>
#include <algorithm>



namespace XX004
{
	const int EventParam::MAX_CACHE_NUMBER = 20;

	stack<EventParam*> EventParam::Caches;

	EventParam::EventParam(): id(0), bool_value(false), int_value1(0), int_value2(0), int_value3(0), int_value4(0)
		, long_value1(0), long_value2(0), ptr_value1(NULL), ptr_value2(NULL)
	{
	}

	EventParam::~EventParam()
	{
	}

	EventParam* EventParam::Get(int id)
	{
		EventParam *ep = NULL;
		if (Caches.size() > 0)
		{
			ep = Caches.top();
			Caches.pop();
			ep->Reset();
		}
		else
		{
			ep = new EventParam();
		}
		ep->id = id;
		return ep;
	}

	void EventParam::Recycle(EventParam *ep)
	{
		if (Caches.size() < MAX_CACHE_NUMBER)
		{
			ep->Reset();
			Caches.push(ep);
		}
		else
		{
			::printf_s("The EventParam cache number has reach max number(%d).\n", MAX_CACHE_NUMBER);
			SAFE_DELETE(ep);
		}
	}

	void EventParam::ReleaseCaches()
	{
		while (Caches.size() > 0)
		{
			EventParam *ep = Caches.top();
			Caches.pop();
			SAFE_DELETE(ep);
		}
	}

	void EventParam::Reset()
	{
		id = 0;
		bool_value = false;
		int_value1 = 0;
		int_value2 = 0;
		int_value3 = 0;
		int_value4 = 0;
		long_value1 = 0;
		long_value2 = 0;
		ptr_value1 = NULL;
		ptr_value2 = NULL;
		string_value1.clear();
		string_value2.clear();
	}

	EventCallBackObject::EventCallBackObject(EventCallBack c, void *p): arg(p), call(c)
	{
	}

	bool EventCallBackObject::operator==(const EventCallBackObject &obj)const
	{
		return this->call == obj.call && this->arg == obj.arg;
	}

	EventManager* EventManager::GetInstance()
	{
		static EventManager instance;
		static bool init = false;
		if (!init)
		{
			instance.Init();
			init = true;
		}
		return &instance;
	}

	void EventManager::RegisterCallBack(int id, EventCallBack call, void *arg)
	{
		EventCallBackVector &callback = m_CallBack[id];
		EventCallBackObject call_obj(call, arg);
		if (find(callback.begin(), callback.end(), call_obj) != callback.end())
		{
			::printf_s("The CallBack(id:%d) is alread registered!\n", id);
			return;
		}
		callback.push_back(EventCallBackObject(call, arg));
	}

	void EventManager::UnregisterCallBack(int id, EventCallBack call, void *arg)
	{
		EventCallBackVector &callback = m_CallBack[id];
		EventCallBackVector::iterator itr = find(callback.begin(), callback.end(), EventCallBackObject(call, arg));
		if (itr != callback.end())
		{
			if (find(m_CurTriggerEvents.begin(), m_CurTriggerEvents.end(), id) == m_CurTriggerEvents.end())
			{
				itr = callback.erase(itr);
			}
			else
			{
				//事件触发中，不能直接删，只能先置NULL，否则会导致触发回调下标遍历错误
				itr->call = NULL;
			}
		}
	}

	void EventManager::UnregisterAllCallBack()
	{
		m_CallBack.clear();
	}

	void EventManager::TriggerEvent(EventParam *ep, bool auto_recycle)
	{
		assert(ep != NULL);
		m_CurTriggerEvents.push_back(ep->id);

		//逐个触发，如果已经为空则记录下来
		EventCallBackVector &callback = m_CallBack[ep->id];
		vector<size_t> need_remove;
		for (size_t i = 0; i < callback.size(); ++i)		//触发过程中可能会对列表进行修改，所以基于下标索引而不是迭代器
		{
			EventCallBackObject call_obj = callback[i];
			if (call_obj.call != NULL)
			{
				call_obj.call(call_obj.arg, ep->id, ep);
			}
			else
			{
				need_remove.push_back(i);
			}
		}

		//移除那些已经为kon的回调
		for (int i = (int)need_remove.size() - 1; i >= 0; --i)
		{
			callback.erase(callback.begin() + need_remove[i]);
		}
		m_CurTriggerEvents.pop_back();
		if (auto_recycle)
		{
			EventParam::Recycle(ep);
		}
	}
	
	EventManager::EventManager()
	{
	}

	EventManager::~EventManager()
	{
		m_CallBack.clear();
	}

	void EventManager::Init()
	{
	}
}