/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: LoginModuleConfig.h
* Summary: 登陆模块配置。
*
* Author: XuXiang
* Date: 2019-01-24 20:38
*******************************************************/

#ifndef __LoginModuleConfig_h__
#define __LoginModuleConfig_h__

#include "ModuleConfig.h"
#include "ConfigDefine.h"

namespace XX004
{
	class LoginModuleConfig : public ModuleConfig
	{
	public:
		LoginModuleConfig();
		virtual ~LoginModuleConfig();

		virtual void Init();
		virtual void Release();
		virtual const char* GetName()const { return "LoginModuleConfig"; }

		const CharacterConfigMap& GetAllCharacter()const { return m_Characters; }
		CharacterConfig* GetCharacter(int id)const { auto itr = m_Characters.find(id); return itr == m_Characters.end() ? NULL : itr->second; }

		const ProfConfigMap& GetAllProf()const { return m_Profs; }
		ProfConfig* GetProf(int id)const { auto itr = m_Profs.find(id); return itr == m_Profs.end() ? NULL : itr->second; }

	protected:

	private:

		//角色配置
		CharacterConfigMap m_Characters;

		//职业配置
		ProfConfigMap m_Profs;
	};
}

#endif
