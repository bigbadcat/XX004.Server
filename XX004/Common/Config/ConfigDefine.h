/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ConfigDefine.h
* Summary: 配置定义。
*
* Author: XuXiang
* Date: 2019-03-13 16:52
*******************************************************/

#ifndef __ConfigDefine_h__
#define __ConfigDefine_h__

#include "../Macro.h"
#include <map>
#include <vector>
using namespace std;

namespace XX004
{
#define CONFIG_MAP(cfg) typedef map<int, cfg*> cfg##Map

	//属性类型
	enum AttrType
	{
		AT_Unknow = 0,				//未知
		AT_MaxHP = 1,				//生命
		AT_MaxEnergy = 2,			//能量
		AT_MaxStability = 3,		//定力
		AT_Attack = 4,				//攻击
		AT_Defense = 5,				//防御
		AT_Hit = 6,					//命中率
		AT_Parry = 7,				//格挡率
		AT_ParryDerate = 8,			//格挡减免
		AT_CritProb = 9,			//暴击率
		AT_CritDamge = 10,			//暴击伤害
		AT_AntiCritProb = 11,		//韧性
		AT_AntiCritDamge = 12,		//暴击减伤
		AT_HPRecover = 13,			//生命恢复
		AT_EnergyRecover = 14,		//能量恢复
		AT_StabilityRecover = 15,	//定力恢复
		AT_HPSuckUp = 16,			//吸血
		AT_HurtBack = 17,			//反伤害
		AT_HurtAdd = 18,			//伤害加成
		AT_HurtSub = 19,			//伤害减少
		AT_Speed = 20,				//速度
	};

	//属性名称到类型
	int attr_name_to_type(const string& str);

	//解析属性集合
	void parse_attr(const string& str, map<int, Int64> attrs);
}

#endif
