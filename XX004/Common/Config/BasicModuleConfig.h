/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: BasicModuleConfig.h
* Summary: 基础模块配置。
*
* Author: XuXiang
* Date: 2019-01-24 20:38
*******************************************************/

#ifndef __BasicModuleConfig_h__
#define __BasicModuleConfig_h__

#include "ModuleConfig.h"
#include "ConfigDefine.h"
#include "LuaWrap/ConstWrap.h"
#include "LuaWrap/ProfWrap.h"
#include "LuaWrap/ProfAttrWrap.h"

namespace XX004
{
	class ConstConfig : public ConstWrap {};
	CONFIG_MAP(ConstConfig);

	class ProfConfig : public ProfWrap {};
	CONFIG_MAP(ProfConfig);

	class ProfAttrConfig : public ProfAttrWrap {};
	CONFIG_MAP(ProfAttrConfig);

	class BasicModuleConfig : public ModuleConfig
	{
	public:
		BasicModuleConfig();
		virtual ~BasicModuleConfig();

		virtual const char* GetName()const { return "BasicModuleConfig"; }

		const ProfConfigMap& GetAllProf()const { return m_Profs; }
		ProfConfig* GetProf(int id)const { auto itr = m_Profs.find(id); return itr == m_Profs.end() ? NULL : itr->second; }

	protected:

		virtual void OnInit();
		virtual void OnRelease();

	private:

		//常量配置
		ConstConfigMap m_Consts;

		//职业配置
		ProfConfigMap m_Profs;

		//职业属性配置
		ProfAttrConfigMap m_ProfAttrs;
	};
}

#endif
