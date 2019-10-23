/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ConstWrap.h
* Summary: Wrap for t_const.lua
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __ConstWrap_h__
#define __ConstWrap_h__

#include "../LuaWrap.h"

namespace XX004
{
	class ConstWrap : public LuaWrap
	{
	public:
		ConstWrap();
		virtual void Init(lua_State *L);

		int id;								//编号
		string name;						//名称
		string value1;						//整数值1
		int value2;							//整数值2
		int value3;							//整数值3
		string value_str;					//字符串值
	};
}

#endif
