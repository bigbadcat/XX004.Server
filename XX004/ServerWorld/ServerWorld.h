/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerWorld.h
* Summary: 世界服务模块。
*
* Author: XuXiang
* Date: 2019-04-12 16:45
*******************************************************/

#ifndef __ServerWorld_h__
#define __ServerWorld_h__

#include "ServerBase.h"

namespace XX004
{
	class ServerWorld : public ServerBase
	{
	public:
		ServerWorld();
		virtual ~ServerWorld();

	protected:
		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr);

		//初始化配置列表
		virtual void OnAddConfig(vector<ModuleConfig*> &cfgs);

		//启动服务
		virtual bool OnInitStep(int step, float &r);

		//停止服务
		virtual bool OnReleaseStep(int step, float &r);

		//帧更新
		virtual void OnUpdate();

		//秒更新
		virtual void OnUpdatePerSecond();

		//收到命令
		virtual void OnCommand(const std::string& cmd, const std::vector<std::string> &param);

	private:
		void OnConnect(NetDataItem *item);
		void OnDisconnect(NetDataItem *item);

		//void OnLoginRequest(NetDataItem *item);
	};
}

#endif