/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ProfWrap.cpp
* Summary: Wrap for t_prof.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "ProfWrap.h"

namespace XX004
{
	ProfWrap::ProfWrap() : id(0)
	{
	}

	void ProfWrap::Init(lua_State *L)
	{
		this->id = LuaWrap::ReadInt(L, "id");
		LuaWrap::ReadStringArray(L, "init_attr", this->init_attr);
		OnParse();
	}
}
