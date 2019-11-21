/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: AttrWrap.h
* Summary: Wrap for t_attr.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __AttrWrap_h__
#define __AttrWrap_h__

#include "../LuaWrap.h"

namespace XX004
{
	class AttrWrap : public LuaWrap
	{
	public:
		AttrWrap();
		virtual void Init(lua_State *L);

		int type;							//类型
		string key;							//缩写
		bool have_runtime;					//有运行时属性
		int add_target;						//加成目标
		int power_rate;						//战力系数
	};
}

#endif
