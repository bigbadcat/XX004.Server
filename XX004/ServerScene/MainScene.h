/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainScene.h
* Summary: 场景服程序入口管理。
*
* Author: XuXiang
* Date: 2019-09-21 16:54
*******************************************************/

#ifndef __MainLogin_h__
#define __MainLogin_h__

#include "MainBase.h"

namespace XX004
{
	class MainScene : public MainBase
	{
	public:
		MainScene();
		virtual ~MainScene();

	protected:
		//创建网络管理
		virtual NetManagerBase* OnCreateNetManager();

		//创建服务模块
		virtual ServerBase* OnCreateServer();

	private:

	};
}

#endif