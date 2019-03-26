/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: LoginModuleConfig.cpp
* Summary: 登陆模块配置。
*
* Author: XuXiang
* Date: 2019-01-24 20:38
*******************************************************/

#include "LoginModuleConfig.h"
#include <iostream>
using namespace std;

namespace XX004
{
	LoginModuleConfig::LoginModuleConfig()
	{
	}

	LoginModuleConfig::~LoginModuleConfig()
	{
		Release();
	}

	void LoginModuleConfig::Init()
	{
		lua_State *L = LuaWrap::GetLuaState();
		ModuleConfig::LoadConfig<CharacterConfig>(m_Characters, L, "t_character");
		ModuleConfig::LoadConfig<ProfConfig>(m_Profs, L, "t_prof");
	}

	void LoginModuleConfig::Release()
	{
		SAFE_DELETE_MAP(m_Characters);
		SAFE_DELETE_MAP(m_Profs);
	}
}