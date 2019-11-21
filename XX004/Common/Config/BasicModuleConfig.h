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
#include "LuaWrap/AttrWrap.h"
#include "LuaWrap/ProfWrap.h"
#include "LuaWrap/ProfLevelWrap.h"
#include "LuaWrap/ProfAttrWrap.h"

namespace XX004
{
	//常量配置
	class ConstConfig : public ConstWrap {};
	CONFIG_MAP(ConstConfig);

	//属性配置
	class AttrConfig : public AttrWrap {};
	CONFIG_MAP(AttrConfig);

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

	//职业等级配置
	class ProfLevelConfig : public ProfLevelWrap {};
	CONFIG_MAP(ProfLevelConfig);

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

		//获取运行时属性
		inline const vector<int>& GetRuntimeAttrs()const { return m_RuntimeAttrs; }

		//通过属性名获取属性类型
		int GetAttrType(const string& name);

		//解析属性 str:name*value,name*value... ret:attrs自身
		map<int, Int64>& ParseAttr(const string& str, map<int, Int64>& attrs);

		//解析属性 str:name*value数组 ret:attrs自身
		map<int, Int64>& ParseAttr(const vector<string>& str, map<int, Int64>& attrs);

		//打印属性 attrs:属性集合 print_zero:是打印值为0的属性
		void PrintAttr(const map<int, Int64>& attrs, bool print_zero = false);

		const ProfConfigMap& GetAllProf()const { return m_Profs; }
		ProfConfig* GetProf(int id)const { auto itr = m_Profs.find(id); return itr == m_Profs.end() ? NULL : itr->second; }

		//获取职业最大等级
		inline int GetProfMaxLevel()const { return m_ProfMaxLevel; }

		//获取职业等级配置
		ProfLevelConfig* GetProfLevel(int level)const { auto itr = m_ProfLevels.find(level); return itr == m_ProfLevels.end() ? NULL : itr->second; }

		//获取新的职业等级 ret:剩余经验
		Int64 GetNewLevel(int old_level, Int64 old_exp, Int64 add_exp, int &new_level);

		//获取从某个等级提升到另一个等级需要的经验
		Int64 GetNeedExp(int from_level, int to_level);

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

		//属性配置
		AttrConfigMap m_Attrs;

		//属性比例类型
		vector<pair<int, int> > m_AttrRateTypes;

		//运行时属性
		vector<int> m_RuntimeAttrs;

		//属性名称到类型
		map<string, int> m_AttrNameToType;

		//职业配置
		ProfConfigMap m_Profs;

		//职业最大等级
		int m_ProfMaxLevel;

		//职业等级配置
		ProfLevelConfigMap m_ProfLevels;

		//职业属性配置
		ProfAttrConfigMap m_ProfAttrs;

		//加速职业等级属性查询
		ProfLevelAttrConfigMap m_ProfLevelAttrs;
	};
}

#endif
