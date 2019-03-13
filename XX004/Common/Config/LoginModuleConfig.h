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

		const CharacterConfigMap& GetAllCharacter()const { return m_Characters; }

		CharacterConfig* GetCharacter(int id)const { CharacterConfigMap::const_iterator itr = m_Characters.find(id); return itr == m_Characters.end() ? NULL : itr->second; }

	protected:

	private:

		//角色配置
		CharacterConfigMap m_Characters;
	};
}

#endif
