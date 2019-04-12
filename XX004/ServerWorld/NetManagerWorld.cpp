/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerWorld.cpp
* Summary: 世界服网络管理。
*
* Author: XuXiang
* Date: 2019-04-12 16:43
*******************************************************/

#include "NetManagerWorld.h"

namespace XX004
{
	NetManagerWorld::NetManagerWorld()
	{
	}

	NetManagerWorld::~NetManagerWorld()
	{
	}

	void NetManagerWorld::OnCreateInternalInfo(std::vector<int> &infos)
	{
		infos.push_back(RemoteType::RT_DATA);
	}
}