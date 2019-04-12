/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerWorld.cpp
* Summary: 世界服务模块。
*
* Author: XuXiang
* Date: 2019-04-12 16:45
*******************************************************/

#include "ServerWorld.h"
#include <NetManagerBase.h>
#include <Protocol/NetProtocol.h>
#include <MainBase.h>
using namespace XX004::Net;

namespace XX004
{
	ServerWorld::ServerWorld()
	{
	}

	ServerWorld::~ServerWorld()
	{
	}

	void ServerWorld::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		//NET_REGISTER(pMgr, NetMsgID::GL_LOGIN_REQ, OnLoginRequest);
	}

	void ServerWorld::OnAddConfig(vector<ModuleConfig*> &cfgs)
	{
	}

	bool ServerWorld::OnInitStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerWorld::OnUpdate()
	{
	}

	void ServerWorld::OnUpdatePerSecond()
	{
	}

	void ServerWorld::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
	}

	bool ServerWorld::OnReleaseStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerWorld::OnConnect(NetDataItem *item)
	{
	}

	void ServerWorld::OnDisconnect(NetDataItem *item)
	{
	}
}