/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainGate.cpp
* Summary: 网关服程序入口管理。
*
* Author: XuXiang
* Date: 2018-10-25 17:26
*******************************************************/

#include "MainGate.h"
#include "NetManagerGate.h"
#include "ServerGate.h"

namespace XX004
{
	MainGate::MainGate()
	{
	}

	MainGate::~MainGate()
	{
	}

	NetManagerBase* MainGate::OnCreateNetManager()
	{
		return new NetManagerGate();
	}

	ServerBase* MainGate::OnCreateServer()
	{
		return new ServerGate();
	}
}