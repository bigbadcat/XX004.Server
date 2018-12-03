/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: StringUtil.h
* Summary: 字符串通用操作。
*
* Author: XuXiang
* Date: 2018-12-03 20:13
*******************************************************/

#ifndef __util_h__
#define __util_h__

#include <string>
#include <vector>
using namespace std;

namespace XX004
{
	class StringUtil
	{
	public:

		//剔除字符串两端的空格
		//s:要剔除空格的字符串(字符串会被修改)
		//ret:字符串参数自身
		static std::string& Trim(std::string &s);

		//拆分字符串
		//s:要拆分的串
		//sp:分隔符
		//v:存储结果的容器
		//ret:存储结果的容器自身
		static vector<string>& Split(std::string &s, std::string& sp, vector<string>& v);

		//拆分字符串
		//s:要拆分的串
		//sp:分隔符
		//ret:拆分结果
		static inline vector<string> Split(std::string &s, std::string& sp){ vector<string> v; return Split(s, sp, v); }

		//拆分字符串为整数数组
		//s:要拆分的串
		//sp:分隔符
		//v:存储结果的容器
		//ret:拆分结果
		static vector<int>& SplitToInt(std::string &s, std::string& sp, vector<int>& v);

		//拆分字符串为整数数组
		//s:要拆分的串
		//sp:分隔符
		//ret:拆分结果
		static inline vector<int> SplitToInt(std::string &s, std::string& sp) { vector<int> v; return SplitToInt(s, sp, v); }
	};
}

#endif	//__util_h__