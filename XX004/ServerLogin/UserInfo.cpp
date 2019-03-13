/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: UserInfo.cpp
* Summary: 登陆的用户信息。
*
* Author: XuXiang
* Date: 2019-03-13 19:58
*******************************************************/

#include "UserInfo.h"

namespace XX004
{
	Int64 UserInfo::GetNewID()
	{
		return 0;
	}

	UserInfo::UserInfo(const string& name) : m_Name(name), m_FreeTime(0)
	{

	}
}