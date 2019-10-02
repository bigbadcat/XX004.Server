/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerGame.cpp
* Summary: 游戏服务模块。
*
* Author: XuXiang
* Date: 2019-10-02 11:05
*******************************************************/

#include "ServerGame.h"
#include <NetManagerBase.h>
using namespace XX004::Net;

namespace XX004
{
	ServerGame::ServerGame()
	{
	}

	ServerGame::~ServerGame()
	{
	}

	void ServerGame::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		//NET_REGISTER(pMgr, NetMsgID::WS_ROLE_ENTER, OnRoleEnter);
		//NET_REGISTER(pMgr, NetMsgID::CS_MOVE_REQ, OnMoveRequest);
	}

	void ServerGame::OnAddConfig(vector<ModuleConfig*> &cfgs)
	{
		//cfgs.push_back(ModuleConfig::GetInstance<LoginModuleConfig>());
	}

	bool ServerGame::OnInitStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerGame::OnUpdate()
	{
	}

	void ServerGame::OnUpdatePerSecond()
	{
	}

	void ServerGame::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
	}

	bool ServerGame::OnReleaseStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerGame::OnConnect(NetDataItem *item)
	{
	}

	void ServerGame::OnDisconnect(NetDataItem *item)
	{
	}

	//void ServerGame::OnRoleEnter(NetDataItem *item)
	//{
	//	WSRoleEnter req;
	//	req.Unpack(item->data, 0);

	//	//创建玩家角色到场景里

	//	//通知进场
	//	WCSceneEnterNotify notify;
	//	notify.Map = req.Map;
	//	notify.PositionX = req.PositionX;
	//	notify.PositionY = req.PositionY;
	//	notify.Direction = req.Direction;
	//	MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, req.RoleID), NetMsgID::WC_SCENE_ENTER, &notify);
	//}

	//void ServerGame::OnMoveRequest(NetDataItem *item)
	//{
	//	CSMoveRequest req;
	//	req.Unpack(item->data, 0);
	//}
}