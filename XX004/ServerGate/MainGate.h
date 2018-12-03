/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainGate.h
* Summary: 网关服程序入口管理。
*
* Author: XuXiang
* Date: 2018-10-25 17:26
*******************************************************/

#ifndef __MainGate_h__
#define __MainGate_h__

#include "MainBase.h"

namespace XX004
{
	class MainGate : public MainBase
	{
	public:
		MainGate();
		virtual ~MainGate();

	protected:
		//创建网络管理
		virtual NetManagerBase* OnCreateNetManager();

		//创建服务模块
		virtual ServerBase* OnCreateServer();

	private:

	};
}

#endif	//__MainGate_h__