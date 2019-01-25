/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: CharacterWrap.h
* Summary: Wrap for t_character.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __CharacterWrap_h__
#define __CharacterWrap_h__

#include "../LuaWrap.h"

namespace XX004
{
	class CharacterWrap : public LuaWrap
	{
	public:
		CharacterWrap();
		virtual void Init(lua_State *L);

		int id;						//编号
		int type;					//类型	
		int hp;						//生命
		int att;					//攻击
		int def;					//防御
	};
}

#endif
