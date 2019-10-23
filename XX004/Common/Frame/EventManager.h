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

		int id;					//事件id 定义事件id时会说明参数含义
		bool b;					//布尔参数值
		int i1;					//整形参数值1
		int i2;					//整形参数值2
		int i3;					//整形参数值3
		int i4;					//整形参数值4
		float f1;				//浮点型形参数值1
		float f2;				//浮点型形参数值2
		Int64 l1;				//长整形参数值1
		Int64 l2;				//长整形参数值2
		void *p1;				//指针参数值1
		void *p2;				//指针参数值2
		string s1;				//字符串参数值1
		string s2;				//字符串参数值1

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

#define EVENT_DEFINE(T, name) static void OnEvent##name(void *arg, int id, EventParam *ep) { ((T*)(arg))->OnEvent##name(id, ep); }\
	void OnEvent##name(int id, EventParam *ep);

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