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
		pMgr->RegisterMessageCallBack(NetMsgID::GL_LOGIN_REQ, [this](NetDataItem *item){this->OnLoginRequest(item); });
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
		if (cmd.compare("cfg") == 0)
		{
			LoginModuleConfig *cfg = ModuleConfig::GetInstance<LoginModuleConfig>();
			if (param.size() > 0)
			{
				int id = ::atoi(param[0].c_str());
				CharacterConfig *char_cfg = cfg->GetCharacter(id);
				if (char_cfg != NULL)
				{
					::printf("id:%d hp:%d\n", id, char_cfg->hp);
				}
				else
				{
					::printf("id:%d is not exist.\n", id);
				}
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
}