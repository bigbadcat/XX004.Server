/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerData.h
* Summary: 数据服务模块。
*
* Author: XuXiang
* Date: 2018-10-25 19:41
*******************************************************/

#ifndef __ServerData_h__
#define __ServerData_h__

#include "Macro.h"
#include "ServerBase.h"

namespace XX004
{
	class ServerData : public ServerBase
	{
	public:
		ServerData();
		virtual ~ServerData();

	protected:
		//注册网络消息
		virtual void RegisterNetMessage(NetManager *pMgr);

		//启动服务
		virtual bool OnStart(int step, float &r);

		//帧更新
		virtual void OnUpdate();

		//秒更新
		virtual void OnUpdatePerSecond();

		//停止服务
		virtual bool OnStop(int step, float &r);

	private:
		void F1(NetDataItem *item);
	};
}

#endif	//__ServerData_h__