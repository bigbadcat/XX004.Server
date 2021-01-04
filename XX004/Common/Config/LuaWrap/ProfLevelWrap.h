/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ProfLevelWrap.h
* Summary: Wrap for t_prof_level.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __ProfLevelWrap_h__
#define __ProfLevelWrap_h__

#include "../LuaWrap.h"

namespace XX004
{
	class ProfLevelWrap : public LuaWrap
	{
	public:
		ProfLevelWrap();
		virtual void Init(lua_State *L);

		int level;							//等级
		Int64 exp;							//经验
	};
}

#endif
