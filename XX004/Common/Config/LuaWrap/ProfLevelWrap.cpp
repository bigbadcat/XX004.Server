/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ProfLevelWrap.cpp
* Summary: Wrap for t_prof_level.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "ProfLevelWrap.h"

namespace XX004
{
	ProfLevelWrap::ProfLevelWrap() : level(0), exp(0)
	{
	}

	void ProfLevelWrap::Init(lua_State *L)
	{
		this->level = LuaWrap::ReadInt(L, "level");
		this->exp = LuaWrap::ReadLong(L, "exp");
		OnParse();
	}
}
