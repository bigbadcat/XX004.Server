/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerScene.cpp
* Summary: 场景服务模块。
*
* Author: XuXiang
* Date: 2019-09-21 16:54
*******************************************************/

#include "ServerScene.h"
#include <NetManagerBase.h>
#include <Protocol/NetProtocol.h>
#include <MainBase.h>
#include <Util/TimeUtil.h>
#include <Util/StringUtil.h>
#include <regex>
using namespace XX004::Net;

namespace XX004
{
	ServerScene::ServerScene()
	{
	}

	ServerScene::~ServerScene()
	{
	}

	void ServerScene::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		NET_REGISTER(pMgr, NetMsgID::WS_ROLE_ENTER, OnRoleEnter);
		NET_REGISTER(pMgr, NetMsgID::CS_MOVE_REQ, OnMoveRequest);
	}

	void ServerScene::OnAddConfig(vector<ModuleConfig*> &cfgs)
	{
		//cfgs.push_back(ModuleConfig::GetInstance<LoginModuleConfig>());
	}

	bool ServerScene::OnInitStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerScene::OnUpdate()
	{
	}

	void ServerScene::OnUpdatePerSecond()
	{
	}

	void ServerScene::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
	}

	bool ServerScene::OnReleaseStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerScene::OnConnect(NetDataItem *item)
	{
		if (item->key.first == RemoteType::RT_DATA)
		{
			LDRoleStampRequest req;
			req.Group = this->GetServerGroup();
			req.ID = this->GetServerID();
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_ROLE_STAMP_REQ, &req);
		}
	}

	void ServerScene::OnDisconnect(NetDataItem *item)
	{
	}

	void ServerScene::OnRoleEnter(NetDataItem *item)
	{
		WSRoleEnter req;
		req.Unpack(item->data, 0);

		//创建玩家角色到场景里

		//通知进场
		WCSceneEnterNotify notify;
		notify.Map = req.Map;
		notify.PositionX = req.PositionX;
		notify.PositionY = req.PositionY;
		notify.Direction = req.Direction;
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, req.RoleID), NetMsgID::WC_SCENE_ENTER, &notify);
	}

	void ServerScene::OnMoveRequest(NetDataItem *item)
	{
		CSMoveRequest req;
		req.Unpack(item->data, 0);
		
		////若是非法用户名直接返回
		//bool ok = true;
		//if (req.UserName.length() < 4 || req.UserName.length() > 16)
		//{
		//	ok = false;
		//}
		//if (ok)
		//{
		//	//正则匹配格式
		//	regex reg("^[a-zA-Z0-9_]*$");
		//	ok = regex_match(req.UserName, reg);
		//}

		////用户名合法则请求用户信息，否则返回用户名非法
		//if (ok)
		//{
		//	UserInfoMap::iterator itr = m_UserInfos.find(req.UserName);
		//	if (itr == m_UserInfos.end())
		//	{
		//		//数据库查询
		//		LDUserInfoRequest req2;
		//		req2.UserName = req.UserName;
		//		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_INFO_REQ, &req);
		//	}
		//	else
		//	{
		//		//将结果回复给玩家
		//		UserInfo* info = itr->second;
		//		Int64 now = (Int64)TimeUtil::GetCurrentSecond();
		//		LGLoginResponse res2;
		//		res2.Result = info->GetFreeTime() <= now ? 0 : 2;
		//		res2.UserName = info->GetName();
		//		res2.FreeTime = info->GetFreeTime();
		//		const vector<LoginRoleInfo>& roles = info->GetRoleInfos();
		//		res2.RoleCount = (Int32)roles.size();
		//		res2.RoleList.assign(roles.begin(), roles.end());
		//		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_LOGIN_RES, &res2);
		//	}
		//}
		//else
		//{
		//	LGLoginResponse res2;
		//	res2.Result = 1;
		//	res2.UserName = req.UserName;
		//	MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_LOGIN_RES, &res2);
		//}		
	}
}