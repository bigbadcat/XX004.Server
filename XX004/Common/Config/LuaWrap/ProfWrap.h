/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ProfWrap.h
* Summary: Wrap for t_prof.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __ProfWrap_h__
#define __ProfWrap_h__

#include "../LuaWrap.h"

namespace XX004
{
	class ProfWrap : public LuaWrap
	{
	public:
		ProfWrap();
		virtual void Init(lua_State *L);

		int id;								//编号
		int hp;								//生命
		int att;							//攻击
		int def;							//防御
	};
}

#endif
