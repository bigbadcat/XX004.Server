/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerGate.h
* Summary: 网关服务模块。
*
* Author: XuXiang
* Date: 2018-12-03 19:42
*******************************************************/

#ifndef __ServerGate_h__
#define __ServerGate_h__

#include "ServerBase.h"

namespace XX004
{
	class ServerGate : public ServerBase
	{
	public:
		ServerGate();
		virtual ~ServerGate();

	protected:
		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr);

		//启动服务
		virtual bool OnInitStep(int step, float &r);

		//停止服务
		virtual bool OnReleaseStep(int step, float &r);

		//帧更新
		virtual void OnUpdate();

		//秒更新
		virtual void OnUpdatePerSecond();

	private:
		void OnConnect(NetDataItem *item);
		void OnDisconnect(NetDataItem *item);
		void F1(NetDataItem *item);
	};
}

#endif	//__ServerGate_h__