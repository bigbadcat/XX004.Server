/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ModuleConfig.cpp
* Summary: 模块配置的基类。
*
* Author: XuXiang
* Date: 2019-01-24 19:18
*******************************************************/

#include "ModuleConfig.h"
#include <lua.hpp>

namespace XX004
{
	ModuleConfig::ModuleConfig()
	{
	}

	ModuleConfig::~ModuleConfig()
	{
	}

	bool ModuleConfig::PrepareRead(lua_State *L, const char *name)
	{
		char file[64];
		::sprintf_s(file, sizeof(file), "Lua/Config/%s.lua", name);
		int top = lua_gettop(L);
		bool ok = LuaWrap::DoLuaFile(L, file, false);
		if (!ok)
		{
			return false;
		}

		int newtop = lua_gettop(L);
		if (newtop - top != 1 || !lua_istable(L, -1))
		{
			lua_settop(L, top);
			printf("The lua file is not return a table. name:%s", name);
			return false;
		}

		lua_pushnil(L);
		return true;
	}

	bool ModuleConfig::ReadRecord(lua_State *L, LuaWrap *wrap, int *key)
	{
		//没有数据了
		if (!lua_next(L, -2))
		{
			return false;
		}

		//读取key
		lua_pushnumber(L, 1);
		lua_gettable(L, -2);
		*key = (int)lua_tonumber(L, -1);
		lua_pop(L, 1);

		wrap->Init(L);
		lua_pop(L, 1);		//把值弹出去
		return true;
	}
}