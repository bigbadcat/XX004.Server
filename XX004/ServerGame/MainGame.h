/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainGame.h
* Summary: 游戏服程序入口管理。
*
* Author: XuXiang
* Date: 2019-10-02 11:05
*******************************************************/

#ifndef __MainGame_h__
#define __MainGame_h__

#include <Frame/MainBase.h>

namespace XX004
{
	class MainGame : public MainBase
	{
	public:
		MainGame();
		virtual ~MainGame();

	protected:
		//创建网络管理
		virtual NetManagerBase* OnCreateNetManager();

		//创建服务模块
		virtual ServerBase* OnCreateServer();

	private:

	};
}

#endif