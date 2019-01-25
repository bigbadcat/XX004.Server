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
	}

	void LoginModuleConfig::Release()
	{
		for (CharacterConfigMap::iterator itr = m_Characters.begin(); itr != m_Characters.end(); ++itr)
		{
			SAFE_DELETE(itr->second);
		}
		m_Characters.clear();
	}
}