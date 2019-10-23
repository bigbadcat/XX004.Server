/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: EventManager.h
* Summary: 事件系统。
*
* Author: XuXiang
* Date: 2019-10-22 17:10
*******************************************************/

#ifndef __EventManager_h__
#define __EventManager_h__

#include "../Macro.h"
#include <map>
#include <vector>
#include <string>
#include <stack>
using namespace std;

namespace XX004
{
	
	//事件参数
	struct EventParam
	{
	public:

		//最大缓存数量
		static const int MAX_CACHE_NUMBER;

		//获取一个参数
		static EventParam* Get(int id);

		//回收一个参数
		static void Recycle(EventParam *ep);

		//释放缓存
		static void ReleaseCaches();

		void Reset();

		int id;
		bool bool_value;
		int int_value1;
		int int_value2;
		int int_value3;
		int int_value4;
		XX004::Int64 long_value1;
		XX004::Int64 long_value2;
		void *ptr_value1;
		void *ptr_value2;
		string string_value1;
		string string_value2;

	private:
		EventParam();
		~EventParam();

		static stack<EventParam*> Caches;
	};

	typedef void(*EventCallBack)(void*, int, EventParam*);
	struct EventCallBackObject
	{
	public:
		EventCallBackObject(EventCallBack c, void *p);

		bool operator==(const EventCallBackObject &obj)const;
		inline bool operator!=(const EventCallBackObject &obj)const { return !(*this == obj); }

		EventCallBack call;
		void *arg;
	};

	typedef std::vector<EventCallBackObject> EventCallBackVector;
	typedef std::map<Int32, EventCallBackVector> EventCallBackMap;

	class EventManager
	{
	public:

		//获取事件管理
		static EventManager* GetInstance();

		//注册事件函数
		void RegisterCallBack(int id, EventCallBack call, void *arg);

		//注销事件函数
		void UnregisterCallBack(int id, EventCallBack call, void *arg);

		//注销所有网络消息处理函数
		void UnregisterAllCallBack();

		//触发一个事件 ep:事件参数 auto_recycle:若为true，ep自动回收不可再使用，false可重复使用，需要自己手动调用EventParam.Recycle对ep回收。
		void TriggerEvent(EventParam *ep, bool auto_recycle = true);

	private:

		EventManager();
		~EventManager();

		//初始化
		void Init();

		//消息集合
		EventCallBackMap m_CallBack;

		//当前正在触发的事件
		vector<int> m_CurTriggerEvents;
	};

}

#endif