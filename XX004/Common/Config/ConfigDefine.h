﻿/*******************************************************
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
		AT_Unknow = 0,						//未知
		AT_MaxHP = 1,						//生命
		AT_MaxEnergy = 2,					//能量
		AT_MaxStability = 3,				//定力
		AT_Attack = 4,						//攻击
		AT_Defense = 5,						//防御
		AT_HPRecover = 6,					//生命恢复
		AT_EnergyRecover = 7,				//能量恢复
		AT_StabilityRecover = 8,			//定力恢复
		AT_Speed = 9,						//速度
		AT_Hit = 10,						//命中率
		AT_Parry = 11,						//格挡率
		AT_ParryDerate = 12,				//格挡减免
		AT_CritProb = 13,					//暴击率
		AT_CritDamge = 14,					//暴击伤害
		AT_AntiCritProb = 15,				//韧性
		AT_AntiCritDamge = 16,				//暴击减伤
		AT_HPSuckUp = 17,					//吸血
		AT_HurtBack = 18,					//伤害反弹
		AT_HurtAdd = 19,					//伤害加成
		AT_HurtSub = 20,					//伤害减免		
		//--------------------------加成类属性--------------------------//
		AT_MaxHPRate = 21,					//生命加成比例(千分比)
		AT_MaxEnergyRate = 22,				//能量加成比例(千分比)
		AT_MaxStabilityRate = 23,			//定力加成比例(千分比)
		AT_AttackRate = 24,					//攻击加成比例(千分比)
		AT_DefenseRate = 25,				//防御加成比例(千分比)		
		AT_HPRecoverRate = 26,				//生命恢复加成(千分比)
		AT_EnergyRecoverRate = 27,			//能量恢复加成(千分比)
		AT_StabilityRecoverRate = 28,		//定力恢复加成(千分比)
		AT_SpeedRate = 29,					//速度加成比例(千分比)
		AT_MAX = 30,						//属性数量
	};
}

#endif
