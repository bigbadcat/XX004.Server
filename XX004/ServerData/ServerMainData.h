/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerMainData.h
* Summary: 数据服程序入口管理。
*
* Author: XuXiang
* Date: 2018-10-25 17:26
*******************************************************/

#ifndef __ServerMainData_h__
#define __ServerMainData_h__

#include "ServerMain.h"

namespace XX004
{
	class ServerMainData : public ServerMain
	{
	public:
		ServerMainData();
		virtual ~ServerMainData();

	protected:
		//创建网络管理
		virtual NetManager* OnCreateNetManager();

		//创建服务模块
		virtual ServerBase* OnCreateServer();

	private:

	};
}

#endif	//__ServerMainData_h__