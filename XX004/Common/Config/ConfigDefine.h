/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ConfigDefine.h
* Summary: 配置定义。
*
* Author: XuXiang
* Date: 2019-03-13 16:52
*******************************************************/

#ifndef __ConfigDefine_h__
#define __ConfigDefine_h__

#include "ModuleConfig.h"
#include "LuaWrap/CharacterWrap.h"
#include "LuaWrap/ProfWrap.h"

namespace XX004
{
#define CONFIG_MAP(cfg) typedef map<int, cfg*> cfg##Map

	class CharacterConfig : public CharacterWrap {};
	CONFIG_MAP(CharacterConfig);

	class ProfConfig : public ProfWrap {};
	CONFIG_MAP(ProfConfig);
}

#endif
