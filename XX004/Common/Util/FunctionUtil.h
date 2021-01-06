/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: FunctionUtil.h
* Summary: 通用操作函数。
*
* Author: FunctionUtil
* Date: 2021-01-06 21:26
*******************************************************/

#ifndef __FunctionUtil_h__
#define __FunctionUtil_h__

#include "../Macro.h"
#include <string>
#include <time.h>
#include <functional>
using namespace std;

namespace XX004
{
	typedef std::function<void(const string&)> CommandCallBack;

	class FunctionUtil
	{
	public:

		//命令循环。
		static void CommandLoop(CommandCallBack call);

	private:

	};
}

#endif