/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerLogin.cpp
* Summary: 登陆网络管理。
*
* Author: XuXiang
* Date: 2018-12-18 19:56
*******************************************************/

#include "NetManagerLogin.h"

namespace XX004
{
	NetManagerLogin::NetManagerLogin()
	{
	}

	NetManagerLogin::~NetManagerLogin()
	{
	}

	void NetManagerLogin::OnCreateInternalInfo(std::vector<int> &infos)
	{
		infos.push_back(RemoteType::RT_DATA);
		infos.push_back(RemoteType::RT_WORLD);
	}
}