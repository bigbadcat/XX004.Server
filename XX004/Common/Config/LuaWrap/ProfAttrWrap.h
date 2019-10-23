/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ProfAttrWrap.h
* Summary: Wrap for t_prof_attr.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __ProfAttrWrap_h__
#define __ProfAttrWrap_h__

#include "../LuaWrap.h"

namespace XX004
{
	class ProfAttrWrap : public LuaWrap
	{
	public:
		ProfAttrWrap();
		virtual void Init(lua_State *L);

		int id;								//编号
		int prof;							//职业
		int level;							//等级
		int hp;								//生命
		int eng;							//能量
		int att;							//攻击
		int def;							//防御
		int hpr;							//回血
	};
}

#endif
