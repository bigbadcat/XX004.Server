/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerMainData.cpp
* Summary: 数据服程序入口管理。
*
* Author: XuXiang
* Date: 2018-10-25 17:26
*******************************************************/

#include "ServerMainData.h"
#include "NetManagerData.h"
#include "ServerData.h"

namespace XX004
{
	ServerMainData::ServerMainData()
	{
	}

	ServerMainData::~ServerMainData()
	{
	}

	NetManager* ServerMainData::OnCreateNetManager()
	{
		return new NetManagerData();
	}

	ServerBase* ServerMainData::OnCreateServer()
	{
		return new ServerData();
	}
}