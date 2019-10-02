/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainGame.cpp
* Summary: 游戏服程序入口管理。
*
* Author: XuXiang
* Date: 2019-10-02 11:05
*******************************************************/

#include "MainGame.h"
#include "NetManagerGame.h"
#include "ServerGame.h"

namespace XX004
{
	MainGame::MainGame()
	{
	}

	MainGame::~MainGame()
	{
	}

	NetManagerBase* MainGame::OnCreateNetManager()
	{
		return new NetManagerGame();
	}

	ServerBase* MainGame::OnCreateServer()
	{
		return new ServerGame();
	}
}