/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainWorld.h
* Summary: 世界服程序入口管理。
*
* Author: XuXiang
* Date: 2019-04-12 16:49
*******************************************************/

#ifndef __MainWorld_h__
#define __MainWorld_h__

#include "MainBase.h"

namespace XX004
{
	class MainWorld : public MainBase
	{
	public:
		MainWorld();
		virtual ~MainWorld();

	protected:
		//创建网络管理
		virtual NetManagerBase* OnCreateNetManager();

		//创建服务模块
		virtual ServerBase* OnCreateServer();

	private:

	};
}

#endif