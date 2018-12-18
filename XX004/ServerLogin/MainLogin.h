/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainLogin.h
* Summary: 登陆服程序入口管理。
*
* Author: XuXiang
* Date: 2018-12-18 19:56
*******************************************************/

#ifndef __MainLogin_h__
#define __MainLogin_h__

#include "MainBase.h"

namespace XX004
{
	class MainLogin : public MainBase
	{
	public:
		MainLogin();
		virtual ~MainLogin();

	protected:
		//创建网络管理
		virtual NetManagerBase* OnCreateNetManager();

		//创建服务模块
		virtual ServerBase* OnCreateServer();

	private:

	};
}

#endif	//__MainLogin_h__