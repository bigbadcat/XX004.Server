/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerScene.cpp
* Summary: 场景服网络管理。
*
* Author: XuXiang
* Date: 2019-09-21 16:54
*******************************************************/

#include "NetManagerScene.h"

namespace XX004
{
	NetManagerScene::NetManagerScene()
	{
	}

	NetManagerScene::~NetManagerScene()
	{
	}

	void NetManagerScene::OnCreateInternalInfo(std::vector<int> &infos)
	{
		infos.push_back(RemoteType::RT_DATA);
		infos.push_back(RemoteType::RT_WORLD);
	}
}