/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ConfigDefine.cpp
* Summary: 配置定义。
*
* Author: XuXiang
* Date: 2019-03-13 16:52
*******************************************************/

#include "ConfigDefine.h"
#include <iostream>
using namespace std;

namespace XX004
{
	int attr_name_to_type(const string& str)
	{
		static map<string, int> name_to_type;
		if (name_to_type.size() == 0)
		{
			name_to_type.insert(make_pair(string("hp"), AttrType::AT_MaxHP));
			name_to_type.insert(make_pair(string("eng"), AttrType::AT_MaxEnergy));
			name_to_type.insert(make_pair(string("stb"), AttrType::AT_MaxStability));
			name_to_type.insert(make_pair(string("att"), AttrType::AT_Attack));
			name_to_type.insert(make_pair(string("def"), AttrType::AT_Defense));
			name_to_type.insert(make_pair(string("hit"), AttrType::AT_Hit));
			name_to_type.insert(make_pair(string("pry"), AttrType::AT_Parry));
			name_to_type.insert(make_pair(string("pryd"), AttrType::AT_ParryDerate));
			name_to_type.insert(make_pair(string("crt"), AttrType::AT_CritProb));
			name_to_type.insert(make_pair(string("crtd"), AttrType::AT_CritDamge));
			name_to_type.insert(make_pair(string("acrt"), AttrType::AT_AntiCritProb));
			name_to_type.insert(make_pair(string("acrtd"), AttrType::AT_AntiCritDamge));
			name_to_type.insert(make_pair(string("hpr"), AttrType::AT_HPRecover));
			name_to_type.insert(make_pair(string("enr"), AttrType::AT_EnergyRecover));
			name_to_type.insert(make_pair(string("stbr"), AttrType::AT_StabilityRecover));
			name_to_type.insert(make_pair(string("suck"), AttrType::AT_HPSuckUp));
			name_to_type.insert(make_pair(string("htb"), AttrType::AT_HurtBack));
			name_to_type.insert(make_pair(string("hta"), AttrType::AT_HurtAdd));
			name_to_type.insert(make_pair(string("hts"), AttrType::AT_HurtSub));
			name_to_type.insert(make_pair(string("spd"), AttrType::AT_Speed));
		}

		map<string, int>::iterator itr = name_to_type.find(str);
		return itr == name_to_type.end() ? AttrType::AT_Unknow : itr->second;
	}

	void parse_attr(const string& str, map<int, Int64> attrs)
	{
		attrs.clear();
	}
}