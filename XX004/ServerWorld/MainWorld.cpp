/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainWorld.cpp
* Summary: 世界服程序入口管理。
*
* Author: XuXiang
* Date: 2019-04-12 16:49
*******************************************************/

#include "MainWorld.h"
#include "NetManagerWorld.h"
#include "ServerWorld.h"

namespace XX004
{
	MainWorld::MainWorld()
	{
	}

	MainWorld::~MainWorld()
	{
	}

	NetManagerBase* MainWorld::OnCreateNetManager()
	{
		return new NetManagerWorld();
	}

	ServerBase* MainWorld::OnCreateServer()
	{
		return new ServerWorld();
	}
}