﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerGame.cpp
* Summary: 游戏服务模块。
*
* Author: XuXiang
* Date: 2019-10-02 11:05
*******************************************************/

#include "ServerGame.h"
#include "Module/Login/LoginModule.h"
#include <NetManagerBase.h>
using namespace XX004::Net;

namespace XX004
{
	ServerGame::ServerGame() : m_LoginModule(NULL)
	{
		InitModules();
		m_LoginModule = GetModule<LoginModule>();
	}

	ServerGame::~ServerGame()
	{
		m_LoginModule = NULL;
		SAFE_DELETE_VECTOR(m_Modules);
	}

	void ServerGame::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		for (ModuleVector::iterator itr = m_Modules.begin(); itr != m_Modules.end(); ++itr)
		{
			(*itr)->RegisterNetMessage(pMgr);
		}
	}

	void ServerGame::RegisterStorageMessage(StorageManager *pMgr)
	{
		for (ModuleVector::iterator itr = m_Modules.begin(); itr != m_Modules.end(); ++itr)
		{
			(*itr)->RegisterStorageMessage(pMgr);
		}
	}

	void ServerGame::OnAddConfig(vector<ModuleConfig*> &cfgs)
	{
		for (ModuleVector::iterator itr = m_Modules.begin(); itr != m_Modules.end(); ++itr)
		{
			(*itr)->AddConfig(cfgs);
		}		
	}

	bool ServerGame::OnInitStep(int step)
	{
		int i = step - 1;
		m_Modules[i]->Init();
		return (i + 1) >= m_Modules.size();
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

	void ServerGame::InitModules()
	{
		m_Modules.push_back(new LoginModule());
	}

	bool ServerGame::OnReleaseStep(int step)
	{
		int i = step - 1;
		m_Modules[i]->Release();
		return (i + 1) >= m_Modules.size();
	}

	void ServerGame::OnConnect(NetDataItem *item)
	{
		m_LoginModule->OnConnect(item);
	}

	void ServerGame::OnDisconnect(NetDataItem *item)
	{
		m_LoginModule->OnDisconnect(item);
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