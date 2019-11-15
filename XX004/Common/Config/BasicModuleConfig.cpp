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
#include <algorithm>
using namespace std;

namespace XX004
{
	void ProfConfig::OnParse()
	{
		ConfigUtil::ParseAttr(init_attr, m_InitAttr);
	}

	void ProfAttrConfig::OnParse()
	{
		m_Attr[AttrType::AT_MaxHP] = this->hp;
		m_Attr[AttrType::AT_MaxEnergy] = this->eng;
		m_Attr[AttrType::AT_Attack] = this->att;
		m_Attr[AttrType::AT_Defense] = this->def;
		m_Attr[AttrType::AT_HPRecover] = this->hpr;
	}

	BasicModuleConfig::BasicModuleConfig() : m_ProfMaxLevel(0)
	{
	}

	BasicModuleConfig::~BasicModuleConfig()
	{
		OnRelease();
	}

	Int64 BasicModuleConfig::GetNewLevel(int old_level, Int64 old_exp, Int64 add_exp, int &new_level)
	{
		assert(old_level > 0 && old_exp >= 0 && add_exp >= 0);
		Int64 last_exp = old_exp + add_exp;
		new_level = old_level;
		while (new_level < m_ProfMaxLevel)
		{
			ProfLevelConfig *cfg = GetProfLevel(new_level + 1);
			if (last_exp < cfg->exp)
			{
				break;
			}
			last_exp -= cfg->exp;
			++new_level;
		}

		//满级后经验不能超过最后一级需要的经验
		new_level = std::min(new_level, m_ProfMaxLevel);
		if (new_level == m_ProfMaxLevel)
		{
			ProfLevelConfig *cfg = GetProfLevel(new_level);
			last_exp = std::min(last_exp, cfg->exp);
		}
		return last_exp;
	}

	Int64 BasicModuleConfig::GetNeedExp(int from_level, int to_level)
	{
		assert(from_level > 0 && to_level > 0);
		Int64 exp = 0;
		to_level = std::min(to_level, m_ProfMaxLevel);
		for (int lv = from_level+1; lv <= to_level; ++lv)
		{
			ProfLevelConfig *cfg = GetProfLevel(lv);
			exp += cfg->exp;
		}
		return exp;
	}

	const map<int, Int64> BasicModuleConfig::GetProfAttr(int prof, int level)const
	{
		//读取职业配置
		assert(level > 0);
		map<int, Int64> attr;
		ProfConfigMap::const_iterator itr = m_Profs.find(prof);
		if (itr == m_Profs.cend())
		{
			::printf_s("BasicModuleConfig::GetProfAttr Bad prof %d\n", prof);
			return attr;
		}

		//读取职业等级配置
		ProfLevelAttrConfigMap::const_iterator pitr = m_ProfLevelAttrs.find(prof);
		ProfLevelAttrConfigMap::mapped_type::const_iterator litr = pitr->second.find(level);
		if (litr == pitr->second.cend())
		{
			::printf_s("BasicModuleConfig::GetProfAttr Bad level %d\n", prof);
			return attr;
		}

		//将初始属性与等级属性相加
		const map<int, Int64>& lattrs = litr->second->GetAttr();
		attr = itr->second->GetInitAttr();
		for (map<int, Int64>::const_iterator it = lattrs.cbegin(); it != lattrs.cend(); ++it)
		{
			attr[it->first] = attr[it->first] + it->second;
		}
		return attr;
	}

	inline Int64 try_get_attr(int type, const map<int, Int64>& attrs)
	{
		map<int, Int64>::const_iterator itr = attrs.find(type);
		return itr == attrs.cend() ? 0 : itr->second;
	}

	map<int, Int64> BasicModuleConfig::CalcFinalAttr(const map<int, Int64>& attrs)const
	{
		//将加成类属性则算回去
		map<int, Int64> fattrs = attrs;
		for (vector<pair<int, int> >::const_iterator itr = m_AttrRateTypes.cbegin(); itr != m_AttrRateTypes.cend(); ++itr)
		{
			fattrs[itr->second] += fattrs[itr->second] * fattrs[itr->first] / 1000;
			fattrs.erase(itr->first);
		}
		return fattrs;
	}

	Int64 BasicModuleConfig::CalcPower(const map<int, Int64>& attrs)const
	{
		Int64 hp = try_get_attr(AttrType::AT_MaxHP, attrs);
		Int64 att = try_get_attr(AttrType::AT_Attack, attrs);
		Int64 def = try_get_attr(AttrType::AT_Defense, attrs);
		return hp / 100 + att + def;
	}

	void BasicModuleConfig::OnInit()
	{
		lua_State *L = LuaWrap::GetLuaState();
		ModuleConfig::LoadConfig<ConstConfig>(m_Consts, L, "t_const");
		ModuleConfig::LoadConfig<ProfConfig>(m_Profs, L, "t_prof");
		ModuleConfig::LoadConfig<ProfLevelConfig>(m_ProfLevels, L, "t_prof_level");
		ModuleConfig::LoadConfig<ProfAttrConfig>(m_ProfAttrs, L, "t_prof_attr");

		//属性比例类型
		m_AttrRateTypes.push_back(make_pair(AttrType::AT_MaxHPRate, AttrType::AT_MaxHP));
		m_AttrRateTypes.push_back(make_pair(AttrType::AT_MaxEnergyRate, AttrType::AT_MaxEnergy));
		m_AttrRateTypes.push_back(make_pair(AttrType::AT_MaxStabilityRate, AttrType::AT_MaxStability));
		m_AttrRateTypes.push_back(make_pair(AttrType::AT_AttackRate, AttrType::AT_Attack));
		m_AttrRateTypes.push_back(make_pair(AttrType::AT_DefenseRate, AttrType::AT_Defense));
		m_AttrRateTypes.push_back(make_pair(AttrType::AT_SpeedRate, AttrType::AT_Speed));

		//二次解析建立缓存
		for (ProfLevelConfigMap::iterator itr = m_ProfLevels.begin(); itr != m_ProfLevels.end(); ++itr)
		{
			m_ProfMaxLevel = std::max(m_ProfMaxLevel, itr->second->level);
		}
		for (ProfAttrConfigMap::iterator itr = m_ProfAttrs.begin(); itr != m_ProfAttrs.end(); ++itr)
		{
			ProfAttrConfig *cfg = itr->second;
			map<int, ProfAttrConfig*>& cfgs = m_ProfLevelAttrs[cfg->prof];
			cfgs[cfg->level] = cfg;
		}
	}

	void BasicModuleConfig::OnRelease()
	{
		m_ProfLevelAttrs.clear();
		SAFE_DELETE_MAP(m_Consts);
		SAFE_DELETE_MAP(m_Profs);
		SAFE_DELETE_MAP(m_ProfLevels);
		m_ProfLevelAttrs.clear();
		SAFE_DELETE_MAP(m_ProfAttrs);
	}
}