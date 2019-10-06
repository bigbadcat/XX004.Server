/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerGame.h
* Summary: 游戏服务模块。
*
* Author: XuXiang
* Date: 2019-10-02 11:05
*******************************************************/

#ifndef __ServerGame_h__
#define __ServerGame_h__

#include <vector>
#include <map>
#include "ServerBase.h"
#include "Module/ModuleBase.h"
using namespace std;

namespace XX004
{
	typedef vector<ModuleBase*> ModuleVector;
	class ServerGame : public ServerBase
	{
	public:
		ServerGame();
		virtual ~ServerGame();

	protected:
		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr);

		//注册存储消息
		virtual void RegisterStorageMessage(StorageManager *pMgr);

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
		void InitModules();

		void OnConnect(NetDataItem *item);
		void OnDisconnect(NetDataItem *item);

		//模块列表
		ModuleVector m_Modules;
	};
}

#endif