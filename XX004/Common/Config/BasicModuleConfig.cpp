/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: BasicModuleConfig.cpp
* Summary: 登陆模块配置。
*
* Author: XuXiang
* Date: 2019-01-24 20:38
*******************************************************/

#include "BasicModuleConfig.h"
#include <iostream>
using namespace std;

namespace XX004
{
	BasicModuleConfig::BasicModuleConfig()
	{
	}

	BasicModuleConfig::~BasicModuleConfig()
	{
		OnRelease();
	}

	void BasicModuleConfig::OnInit()
	{
		lua_State *L = LuaWrap::GetLuaState();
		ModuleConfig::LoadConfig<ConstConfig>(m_Consts, L, "t_const");
		ModuleConfig::LoadConfig<ProfConfig>(m_Profs, L, "t_prof");
		ModuleConfig::LoadConfig<ProfAttrConfig>(m_ProfAttrs, L, "t_prof_attr");
	}

	void BasicModuleConfig::OnRelease()
	{
		SAFE_DELETE_MAP(m_Consts);
		SAFE_DELETE_MAP(m_Profs);
		SAFE_DELETE_MAP(m_ProfAttrs);
	}
}