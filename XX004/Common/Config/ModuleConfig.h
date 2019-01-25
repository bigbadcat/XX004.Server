/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ModuleConfig.h
* Summary: 模块配置的基类。
*
* Author: XuXiang
* Date: 2019-01-24 19:18
*******************************************************/

#ifndef __ModuleConfig_h__
#define __ModuleConfig_h__

#include "LuaWrap.h"
#include "../Macro.h"
#include <map>
#include <vector>
using namespace std;

namespace XX004
{
	class ModuleConfig
	{
	public:
		ModuleConfig();
		virtual ~ModuleConfig();

		//初始化
		virtual void Init() = 0;

		//销毁
		virtual void Release() = 0;

		//获取实例
		template<class T>
		static T* GetInstance();

		//加载配置
		//cfg:保存配置的集合
		//L:Lua状态，函数调用后保证Lua堆栈不变
		//file:Lua配置文件
		template<class T>
		static void LoadConfig(map<int, T*> &cfgs, lua_State *L, const char *name);

	private:

		//准备读取
		static bool PrepareRead(lua_State *L, const char *name);

		//读取一条记录
		static bool ReadRecord(lua_State *L, LuaWrap *wrap, int *key);
	};

	template<class T>
	T* ModuleConfig::GetInstance()
	{
		static T t;
		return &t;
	}

	template<class T>
	void ModuleConfig::LoadConfig(map<int, T*> &cfgs, lua_State *L, const char *name)
	{
		if (!PrepareRead(L, name))
		{
			return;
		}

		int key = 0;
		while (true)
		{
			LuaWrap *wrap = LuaWrap::OnNew<T>();
			if (ReadRecord(L, wrap, &key))
			{
				T *t = dynamic_cast<T*>(wrap);
				cfgs.insert(make_pair(key, t));
			}
			else
			{
				SAFE_DELETE(wrap);		//没读取到就删掉吧
				break;
			}
		}
	}
}

#endif
