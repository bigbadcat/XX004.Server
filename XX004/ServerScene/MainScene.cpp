/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainScene.cpp
* Summary: 场景服程序入口管理。
*
* Author: XuXiang
* Date: 2019-09-21 16:54
*******************************************************/

#include "MainScene.h"
#include "NetManagerScene.h"
#include "ServerScene.h"

namespace XX004
{
	MainScene::MainScene()
	{
	}

	MainScene::~MainScene()
	{
	}

	NetManagerBase* MainScene::OnCreateNetManager()
	{
		return new NetManagerScene();
	}

	ServerBase* MainScene::OnCreateServer()
	{
		return new ServerScene();
	}
}