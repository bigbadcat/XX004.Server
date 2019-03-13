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
	ProfWrap::ProfWrap() : id(0), hp(0), att(0), def(0)
	{
	}

	void ProfWrap::Init(lua_State *L)
	{
		this->id = LuaWrap::ReadInt(L, "id");
		this->hp = LuaWrap::ReadInt(L, "hp");
		this->att = LuaWrap::ReadInt(L, "att");
		this->def = LuaWrap::ReadInt(L, "def");
		OnParse();
	}
}
