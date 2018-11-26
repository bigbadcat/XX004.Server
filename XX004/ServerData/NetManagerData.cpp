/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerData.cpp
* Summary: 数据服网络管理。
*
* Author: XuXiang
* Date: 2018-10-25 17:54
*******************************************************/

#include "NetManagerData.h"

namespace XX004
{
	NetManagerData::NetManagerData()
	{
	}

	NetManagerData::~NetManagerData()
	{
	}

	void NetManagerData::OnCreateInternalInfo(std::vector<InternalInfo> &infos)
	{
		infos.push_back(InternalInfo(RemoteType::RT_DATA, std::make_pair(string("127.0.0.1"), 9000)));
	}
}