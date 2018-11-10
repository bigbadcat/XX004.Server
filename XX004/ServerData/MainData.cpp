/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainData.cpp
* Summary: 数据服程序入口管理。
*
* Author: XuXiang
* Date: 2018-10-25 17:26
*******************************************************/

#include "MainData.h"
#include "NetManagerData.h"
#include "ServerData.h"

namespace XX004
{
	MainData::MainData()
	{
	}

	MainData::~MainData()
	{
	}

	NetManagerBase* MainData::OnCreateNetManager()
	{
		return new NetManagerData();
	}

	ServerBase* MainData::OnCreateServer()
	{
		return new ServerData();
	}
}