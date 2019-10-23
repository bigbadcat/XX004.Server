/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ProfAttrWrap.cpp
* Summary: Wrap for t_prof_attr.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "ProfAttrWrap.h"

namespace XX004
{
	ProfAttrWrap::ProfAttrWrap() : id(0), prof(0), level(0), hp(0), eng(0), att(0), def(0), hpr(0)
	{
	}

	void ProfAttrWrap::Init(lua_State *L)
	{
		this->id = LuaWrap::ReadInt(L, "id");
		this->prof = LuaWrap::ReadInt(L, "prof");
		this->level = LuaWrap::ReadInt(L, "level");
		this->hp = LuaWrap::ReadInt(L, "hp");
		this->eng = LuaWrap::ReadInt(L, "eng");
		this->att = LuaWrap::ReadInt(L, "att");
		this->def = LuaWrap::ReadInt(L, "def");
		this->hpr = LuaWrap::ReadInt(L, "hpr");
		OnParse();
	}
}
