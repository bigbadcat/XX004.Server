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
#include "../Util/StringUtil.h"
#include <iostream>
#include <algorithm>
using namespace std;

namespace XX004
{
	void ProfConfig::OnParse()
	{
		ModuleConfig::GetInstance<BasicModuleConfig>()->ParseAttr(init_attr, m_InitAttr);
	}

	void ProfAttrConfig::OnParse()
	{
		m_Attr[AttrType::AT_MaxHP] = this->hp;
		m_Attr[AttrType::AT_MaxEnergy] = this->eng;
		m_Attr[AttrType::AT_Attack] = this->att;
		m_Attr[AttrType::AT_Defense] = this->def;
		m_Attr[AttrType::AT_HPRecover] = this->hpr;
	}

	BasicModuleConfig::BasicModuleConfig() : m_PowerRate(0), m_ProfMaxLevel(0)
	{
	}

	BasicModuleConfig::~BasicModuleConfig()
	{
		OnRelease();
	}

	int BasicModuleConfig::GetAttrType(const string& name)
	{
		map<string, int>::iterator itr = m_AttrNameToType.find(name);
		return itr == m_AttrNameToType.end() ? AttrType::AT_Unknow : itr->second;
	}

	map<int, Int64>& BasicModuleConfig::ParseAttr(const string& str, map<int, Int64>& attrs)
	{
		vector<string> v;
		StringUtil::Split(str, ",", v);
		return ParseAttr(v, attrs);
	}

	map<int, Int64>& BasicModuleConfig::ParseAttr(const vector<string>& str, map<int, Int64>& attrs)
	{
		vector<string> attr_value;
		attrs.clear();
		for (vector<string>::const_iterator itr = str.cbegin(); itr != str.cend(); ++itr)
		{
			attr_value.clear();
			StringUtil::Split(*itr, "*", attr_value);

			int key = GetAttrType(attr_value[0]);
			Int64 value = ::_atoi64(attr_value[1].c_str());
			attrs.insert(make_pair(key, value));
		}
		return attrs;
	}

	void set_string_width(char *buff, int size, const char *str, int width)
	{
		int len = ::sprintf_s(buff, size, str);
		for (int i = len; i < width; ++i)
		{
			buff[i] = ' ';
		}
		buff[width] = '\0';
	}

	void BasicModuleConfig::PrintAttr(const map<int, Int64>& attrs, bool print_zero)
	{
		char szName[16];
		char szValue[16];
		char szValueWidth[16];
		::printf_s("O-----------attr-----------------O\n");
		for (int i = 1; i < AttrType::AT_MAX; ++i)
		{
			map<int, Int64>::const_iterator itr = attrs.find(i);
			if (itr != attrs.end() && (print_zero || itr->second > 0))
			{
				set_string_width(szName, sizeof(szName), m_Attrs[itr->first]->key.c_str(), 8);
				::sprintf_s(szValue, "%I64d", itr->second);
				set_string_width(szValueWidth, sizeof(szValueWidth), szValue, 14);
				::printf_s("|  %s ----  %s |\n", szName, szValueWidth);
			}
		}
		::printf_s("O--------------------------------O\n");
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

	inline Int64 get_attr_power(int type, const map<int, Int64>& attrs)
	{
		map<int, Int64>::const_iterator itr = attrs.find(type);
		if (itr == attrs.cend())
		{
			return 0;
		}

		AttrConfig *cfg = ModuleConfig::GetInstance<BasicModuleConfig>()->GetAttr(type);
		if (cfg == NULL)
		{
			return 0;
		}

		Int64 value = itr->second * cfg->power_rate / 1000;
		return value;
	}

	Int64 BasicModuleConfig::CalcPower(const map<int, Int64>& attrs)const
	{
		//生命战力
		Int64 hp = get_attr_power(AttrType::AT_MaxHP, attrs);
		Int64 hts = get_attr_power(AttrType::AT_HurtSub, attrs);
		Int64 suck = get_attr_power(AttrType::AT_HPSuckUp, attrs);
		Int64 hpr = get_attr_power(AttrType::AT_HPRecover, attrs);
		Int64 att = get_attr_power(AttrType::AT_Attack, attrs);
		Int64 hp_power = (hp + att*suck / 1000 + hpr) * (1000+ hts) / 1000;

		//攻击战力
		Int64 hta = get_attr_power(AttrType::AT_HurtAdd, attrs);
		Int64 hit = get_attr_power(AttrType::AT_Hit, attrs);
		Int64 crt = get_attr_power(AttrType::AT_CritProb, attrs);
		Int64 crtd = get_attr_power(AttrType::AT_CritDamge, attrs);
		Int64 htb = get_attr_power(AttrType::AT_HurtBack, attrs);
		Int64 eng = get_attr_power(AttrType::AT_MaxEnergy, attrs);
		Int64 enr = get_attr_power(AttrType::AT_EnergyRecover, attrs);
		Int64 eng_rate = 1000 + eng + enr;
		Int64 att_power = (att*eng_rate / 1000)*hit / 1000 * (1000 + crt*crtd / 1000) / 1000 * (1000 + hta) / 1000 + hp*htb / 1000;

		//防御战力
		Int64 def = get_attr_power(AttrType::AT_Defense, attrs);
		Int64 pry = get_attr_power(AttrType::AT_Parry, attrs);
		Int64 pryd = get_attr_power(AttrType::AT_ParryDerate, attrs);
		Int64 acrt = get_attr_power(AttrType::AT_AntiCritProb, attrs);
		Int64 acrtd = get_attr_power(AttrType::AT_AntiCritDamge, attrs);
		Int64 stb = get_attr_power(AttrType::AT_MaxStability, attrs);
		Int64 stbr = get_attr_power(AttrType::AT_StabilityRecover, attrs);
		Int64 def_power = def + hp*(stb*pry / 1000 * pryd / 1000 + acrt*acrtd / 1000) / 1000;

		//总和
		Int64 power = (hp_power + att_power + def_power) * m_PowerRate / 1000;
		return power;
	}

	void BasicModuleConfig::OnInit(lua_State *L)
	{
		//常量
		ModuleConfig::LoadConfig<ConstConfig>(m_Consts, L, "t_const");
		for (ConstConfigMap::iterator itr = m_Consts.begin(); itr != m_Consts.end(); ++itr)
		{
			m_NameToConst.insert(make_pair(itr->second->name, itr->second));
		}
		m_PowerRate = GetConstValue1("power_rate");

		//属性
		ModuleConfig::LoadConfig<AttrConfig>(m_Attrs, L, "t_attr");
		for (AttrConfigMap::iterator itr = m_Attrs.begin(); itr != m_Attrs.end(); ++itr)
		{
			AttrConfig *cfg = itr->second;
			if (cfg->add_target != 0)
			{
				m_AttrRateTypes.push_back(make_pair(cfg->type, cfg->add_target));
			}
			if (cfg->have_runtime)
			{
				m_RuntimeAttrs.push_back(cfg->type);
			}
			m_AttrNameToType.insert(make_pair(cfg->key, cfg->type));
		}

		//职业
		ModuleConfig::LoadConfig<ProfConfig>(m_Profs, L, "t_prof");
		ModuleConfig::LoadConfig<ProfLevelConfig>(m_ProfLevels, L, "t_prof_level");
		ModuleConfig::LoadConfig<ProfAttrConfig>(m_ProfAttrs, L, "t_prof_attr");
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
		m_NameToConst.clear();
		SAFE_DELETE_MAP(m_Consts);
		SAFE_DELETE_MAP(m_Attrs);
		SAFE_DELETE_MAP(m_Profs);
		SAFE_DELETE_MAP(m_ProfLevels);
		m_ProfLevelAttrs.clear();
		SAFE_DELETE_MAP(m_ProfAttrs);
	}
}