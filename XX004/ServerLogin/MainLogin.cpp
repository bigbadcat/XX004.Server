/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainLogin.cpp
* Summary: 登陆服程序入口管理。
*
* Author: XuXiang
* Date: 2018-12-18 19:56
*******************************************************/

#include "MainLogin.h"
#include "NetManagerLogin.h"
#include "ServerLogin.h"

namespace XX004
{
	MainLogin::MainLogin()
	{
	}

	MainLogin::~MainLogin()
	{
	}

	NetManagerBase* MainLogin::OnCreateNetManager()
	{
		return new NetManagerLogin();
	}

	ServerBase* MainLogin::OnCreateServer()
	{
		return new ServerLogin();
	}
}