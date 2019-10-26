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
#include "../Util/StringUtil.h"
#include <assert.h>
#include <iostream>
using namespace std;

namespace XX004
{
	string ConfigUtil::AttrName[AttrType::AT_MAX] = {"", "hp", "eng" , "stb", "att", "def", "hit", "pry", "pryd", 
		"crt", "crtd", "acrt", "acrtd", "hpr", "enr", "stbr", "suck", "htb", "hta", "hts", "spd",
		"hp_r", "eng_r", "stb_r", "att_r", "def_r", "spd_r"};

	const string& ConfigUtil::GetAttrName(int type)
	{
		assert(type > 0 && type < AttrType::AT_MAX);
		return AttrName[type];
	}

	int ConfigUtil::GetAttrType(const string& name)
	{
		static map<string, int> name_to_type;
		if (name_to_type.size() == 0)
		{
			for (int i = 1; i < AttrType::AT_MAX; ++i)
			{
				name_to_type.insert(make_pair(AttrName[i], i));
			}
		}

		map<string, int>::iterator itr = name_to_type.find(name);
		return itr == name_to_type.end() ? AttrType::AT_Unknow : itr->second;
	}

	map<int, Int64>& ConfigUtil::ParseAttr(const string& str, map<int, Int64>& attrs)
	{
		vector<string> v;
		StringUtil::Split(str, ",", v);
		return ParseAttr(v, attrs);
	}

	map<int, Int64>& ConfigUtil::ParseAttr(const vector<string>& str, map<int, Int64>& attrs)
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

	void ConfigUtil::PrintAttr(const map<int, Int64>& attrs, bool print_zero)
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
				set_string_width(szName, sizeof(szName), GetAttrName(itr->first).c_str(), 8);
				::sprintf_s(szValue, "%I64d", itr->second);
				set_string_width(szValueWidth, sizeof(szValueWidth), szValue, 14);
				::printf_s("|  %s ----  %s |\n", szName, szValueWidth);
			}
		}
		::printf_s("O--------------------------------O\n");
	}
}