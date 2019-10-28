﻿/*******************************************************
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
	//常量配置
	class ConstConfig : public ConstWrap {};
	CONFIG_MAP(ConstConfig);

	//职业配置
	class ProfConfig : public ProfWrap
	{
	public:

		//获取初始属性
		const map<int, Int64>& GetInitAttr()const { return m_InitAttr; }

	protected:

		//二次解析
		virtual void OnParse();

	private:
		map<int, Int64> m_InitAttr;
	};
	CONFIG_MAP(ProfConfig);

	//职业等级属性配置
	class ProfAttrConfig : public ProfAttrWrap
	{
	public:

		//获取初始属性
		const map<int, Int64>& GetAttr()const { return m_Attr; }

	protected:

		//二次解析
		virtual void OnParse();

	private:
		map<int, Int64> m_Attr;
	};
	CONFIG_MAP(ProfAttrConfig);
	typedef map<int, map<int, ProfAttrConfig*> > ProfLevelAttrConfigMap;

	//基础模块配置
	class BasicModuleConfig : public ModuleConfig
	{
	public:
		BasicModuleConfig();
		virtual ~BasicModuleConfig();

		virtual const char* GetName()const { return "BasicModuleConfig"; }

		const ProfConfigMap& GetAllProf()const { return m_Profs; }
		ProfConfig* GetProf(int id)const { auto itr = m_Profs.find(id); return itr == m_Profs.end() ? NULL : itr->second; }

		//获取职业对应等级属性
		const map<int, Int64> GetProfAttr(int prof, int level)const;

		//计算最终属性
		map<int, Int64> CalcFinalAttr(const map<int, Int64>& attrs)const;

		//计算战力
		Int64 CalcPower(const map<int, Int64>& attrs)const;

	protected:

		virtual void OnInit();
		virtual void OnRelease();

	private:

		//常量配置
		ConstConfigMap m_Consts;

		//属性比例类型
		vector<pair<int, int> > m_AttrRateTypes;

		//职业配置
		ProfConfigMap m_Profs;

		//职业属性配置
		ProfAttrConfigMap m_ProfAttrs;

		//加速职业等级属性查询
		ProfLevelAttrConfigMap m_ProfLevelAttrs;
	};
}

#endif