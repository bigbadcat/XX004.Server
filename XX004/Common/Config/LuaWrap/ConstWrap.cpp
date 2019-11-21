/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ConstWrap.cpp
* Summary: Wrap for t_const.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "ConstWrap.h"

namespace XX004
{
	ConstWrap::ConstWrap() : id(0), value1(0), value2(0), value3(0)
	{
	}

	void ConstWrap::Init(lua_State *L)
	{
		this->id = LuaWrap::ReadInt(L, "id");
		LuaWrap::ReadString(L, "name", this->name);
		this->value1 = LuaWrap::ReadInt(L, "value1");
		this->value2 = LuaWrap::ReadInt(L, "value2");
		this->value3 = LuaWrap::ReadInt(L, "value3");
		LuaWrap::ReadString(L, "value_str", this->value_str);
		OnParse();
	}
}
