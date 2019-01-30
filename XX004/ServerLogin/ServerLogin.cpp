/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerLogin.cpp
* Summary: 网关服务模块。
*
* Author: XuXiang
* Date: 2018-12-18 19:56
*******************************************************/

#include "ServerLogin.h"
#include <NetManagerBase.h>
#include <Protocol/NetProtocol.h>
#include <MainBase.h>
#include <Config/LoginModuleConfig.h>
#include <Util/TimeUtil.h>
using namespace XX004::Net;

namespace XX004
{
	ServerLogin::ServerLogin()
	{
	}

	ServerLogin::~ServerLogin()
	{
	}

	void ServerLogin::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		NET_REGISTER(pMgr, NetMsgID::GL_LOGIN_REQ, OnLoginRequest);
		NET_REGISTER(pMgr, NetMsgID::DL_USER_INFO_RES, OnUserInfoResponse);
	}

	void ServerLogin::OnAddConfig(vector<ModuleConfig*> &cfgs)
	{
		cfgs.push_back(ModuleConfig::GetInstance<LoginModuleConfig>());
	}

	bool ServerLogin::OnInitStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerLogin::OnUpdate()
	{
	}

	void ServerLogin::OnUpdatePerSecond()
	{
	}

	void ServerLogin::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
	}

	bool ServerLogin::OnReleaseStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerLogin::OnConnect(NetDataItem *item)
	{
	}

	void ServerLogin::OnDisconnect(NetDataItem *item)
	{
	}

	void ServerLogin::OnLoginRequest(NetDataItem *item)
	{
		GLLoginRequest req;
		req.Unpack(item->data, 0);
		
		//若是非法用户名直接返回

		//数据库查询
		LDUserInfoRequest req2;
		req2.UserName = req.UserName;
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_INFO_REQ, &req);
	}

	void ServerLogin::OnUserInfoResponse(NetDataItem *item)
	{
		DLUserInfoResponse res;
		res.Unpack(item->data, 0);

		//账号不存在则自动创建
		if (res.CreateTime == 0)
		{
			//提交保存
			LDUserSaveRequest req;
			req.UserName = res.UserName;
			req.CreateTime = TimeUtil::GetCurrentSecond();
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_SAVE_REQ, &req);

			//重新请求，走正确时序流程
			LDUserInfoRequest req2;
			req2.UserName = res.UserName;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_INFO_REQ, &req2);
		}
		else
		{
			//将结果回复给玩家
			LGLoginResponse res2;
			res2.Result = res.FreeTime == 0 ? 0 : 2;
			res2.UserName = res.UserName;
			res2.FreeTime = res.FreeTime;
			res2.RoleCount = res.RoleCount;
			res2.RoleList.assign(res.RoleList.begin(), res.RoleList.end());
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_LOGIN_RES, &res2);
		}
	}
}