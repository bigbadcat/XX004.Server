/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: CharacterWrap.cpp
* Summary: Wrap for t_character.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "CharacterWrap.h"

namespace XX004
{
	CharacterWrap::CharacterWrap() : id(0), type(0), hp(0), att(0), def(0)
	{
	}

	void CharacterWrap::Init(lua_State *L)
	{
		this->id = LuaWrap::ReadInt(L, "id");
		this->type = LuaWrap::ReadInt(L, "type");
		this->hp = LuaWrap::ReadInt(L, "hp");
		this->att = LuaWrap::ReadInt(L, "att");
		this->def = LuaWrap::ReadInt(L, "def");
		OnParse();
	}
}
