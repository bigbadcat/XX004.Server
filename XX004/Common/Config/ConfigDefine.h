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

	//属性类型，值必须与[S_属性.xlsx]中的类型一致。
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
		AT_HurtBack = 17,			//伤害反弹
		AT_HurtAdd = 18,			//伤害加成
		AT_HurtSub = 19,			//伤害减免
		AT_Speed = 20,				//速度
		//--------------------------加成类属性--------------------------//
		AT_MaxHPRate = 21,			//生命加成比例(千分比)
		AT_MaxEnergyRate = 22,		//能量加成比例(千分比)
		AT_MaxStabilityRate = 23,	//定力加成比例(千分比)
		AT_AttackRate = 24,			//攻击加成比例(千分比)
		AT_DefenseRate = 25,		//防御加成比例(千分比)
		AT_SpeedRate = 26,			//速度加成比例(千分比)
		AT_MAX = 27,				//属性数量
	};
}

#endif
