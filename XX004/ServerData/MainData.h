/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainData.h
* Summary: 数据服程序入口管理。
*
* Author: XuXiang
* Date: 2018-10-25 17:26
*******************************************************/

#ifndef __MainData_h__
#define __MainData_h__

#include "MainBase.h"

namespace XX004
{
	class MainData : public MainBase
	{
	public:
		MainData();
		virtual ~MainData();

	protected:
		//创建网络管理
		virtual NetManagerBase* OnCreateNetManager();

		//创建服务模块
		virtual ServerBase* OnCreateServer();

	private:

	};
}

#endif	//__MainData_h__