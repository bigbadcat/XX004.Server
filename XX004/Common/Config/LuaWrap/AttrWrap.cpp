/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: AttrWrap.cpp
* Summary: Wrap for t_attr.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "AttrWrap.h"

namespace XX004
{
	AttrWrap::AttrWrap() : type(0), have_runtime(false), power_rate(0)
	{
	}

	void AttrWrap::Init(lua_State *L)
	{
		this->type = LuaWrap::ReadInt(L, "type");
		LuaWrap::ReadString(L, "key", this->key);
		this->have_runtime = LuaWrap::ReadBool(L, "have_runtime");
		LuaWrap::ReadString(L, "add_target", this->add_target);
		this->power_rate = LuaWrap::ReadInt(L, "power_rate");
		OnParse();
	}
}
