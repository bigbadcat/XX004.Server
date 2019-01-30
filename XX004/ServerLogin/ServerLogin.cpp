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
		if (cmd.compare("userinfo") == 0)
		{
			if (param.size() > 0)
			{
				LDUserInfoRequest req;
				req.UserName = param[0];
				MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_INFO_REQ, &req);
			}			
		}
		else if (cmd.compare("usersave") == 0)
		{
			if (param.size() > 0)
			{
				LDUserSaveRequest req;
				req.UserName = param[0];
				req.CreateTime = TimeUtil::GetCurrentSecond();
				MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_SAVE_REQ, &req);
			}
		}
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
		cout << "ServerLogin::OnLoginRequest username:" << req.UserName << endl;

		//测试直接返回
		LGLoginResponse res;
		res.Result = 0;
		res.UserName = req.UserName;
		res.FreeTime = 0;
		res.RoleCount = 2;
		for (int i = 0; i < res.RoleCount; ++i)
		{
			LoginRoleInfo info;
			char szName[32];
			::sprintf_s(szName, sizeof(szName), "RoleName%d", i + 1);
			info.ID = 1000 + i + 1;
			info.Name = szName;
			info.Level = 1 + i * 3;
			res.RoleList.push_back(info);
		}
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_LOGIN_RES, &res);
	}

	void ServerLogin::OnUserInfoResponse(NetDataItem *item)
	{
		DLUserInfoResponse res;
		res.Unpack(item->data, 0);

		//账号不存在则自动创建
		if (res.CreateTime == 0)
		{
			LDUserSaveRequest req;
			req.UserName = res.UserName;
			req.CreateTime = TimeUtil::GetCurrentSecond();
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_SAVE_REQ, &req);

			//重新请求
			LDUserInfoRequest req2;
			req2.UserName = res.UserName;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_INFO_REQ, &req2);
		}
		else
		{
			//将结果回复给玩家
			cout << "user:" << res.UserName << " CreateTime:" << res.CreateTime << " FreeTime:" << res.FreeTime << " RoleNum:" << res.RoleCount << endl;
			for (int i = 0; i < res.RoleCount; ++i)
			{
				LoginRoleInfo &role = res.RoleList[i];
				cout << "  id:" << role.ID << " name:" << role.Name << " Prof:" << role.Prof << " CreateTime:" << role.CreateTime << endl;
			}
		}
	}
}