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
#include "Player/PlayerData.h"
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
		NET_REGISTER(pMgr, NetMsgID::LW_ROLE_ONLINE, OnRoleOnlineNotify);
		NET_REGISTER(pMgr, NetMsgID::DW_ROLE_BASE_INFO_REQ, OnRoleBaseInfoResponse);
		NET_REGISTER(pMgr, NetMsgID::GW_ROLE_OUTLINE, OnRoleOutlineNotify);
		NET_REGISTER(pMgr, NetMsgID::GW_ROLE_QUIT, OnRoleQuitNotify);
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
		if (item->key.first == RemoteType::RT_DATA)
		{
			cout << "Request WorldServer Data" << endl;
		}
	}

	void ServerWorld::OnDisconnect(NetDataItem *item)
	{
	}

	void ServerWorld::OnRoleOnlineNotify(NetDataItem *item)
	{
		LWRoleOnline notify;
		notify.Unpack(item->data, 0);

		//向数据库请求玩家的所有数据
		WDRoleBaseInfoRequest req;
		req.UserName = notify.UserName;
		req.ID = notify.RoleID;
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::WD_ROLE_BASE_INFO_REQ, &req);
	}

	void ServerWorld::OnRoleBaseInfoResponse(NetDataItem *item)
	{
		DWRoleBaseInfoResponse res;
		res.Unpack(item->data, 0);
		cout << "OnRoleBaseInfoResponse roleid:" << res.ID << endl;
		

		//通知进入游戏成功		
		PlayerData *player = m_PlayerManager.AddPlayer(&res);
		WGEnterGameSuccess res2;
		res2.UserName = res.UserName;
		res2.RoleID = res.ID;
		player->Send(NetMsgID::WG_ENTER_GAME_SUCCESS, &res2);		

		//进入最后场景
	}

	void ServerWorld::OnRoleOutlineNotify(NetDataItem *item)
	{

	}

	void ServerWorld::OnRoleQuitNotify(NetDataItem *item)
	{

	}
}