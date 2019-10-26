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
#include "HttpGame.h"
#include "Module/Login/LoginModule.h"
#include "Module/Player/PlayerModule.h"
#include <Frame/NetManagerBase.h>
#include <Frame/StartSetting.h>
#include <Frame/ModuleBase.h>
using namespace XX004::Net;

namespace XX004
{
	ServerGame::ServerGame() : m_LoginModule(NULL), m_HttpGame(NULL)
	{
		InitModules();
		m_LoginModule = GetModule<LoginModule>();
		for (ModuleVector::iterator itr = m_Modules.begin(); itr != m_Modules.end(); ++itr)
		{
			if ((*itr)->IsEnableUpdate())
			{
				m_UpdateModules.push_back(*itr);
			}			
		}		
	}

	ServerGame::~ServerGame()
	{
		SAFE_DELETE(m_HttpGame);
		m_LoginModule = NULL;
		m_UpdateModules.clear();
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
		//先初始化模块
		if (step <= m_Modules.size())
		{
			m_Modules[step - 1]->Init();
			return false;
		}

		//启动Http服务
		ServerSetting* info = StartSetting::GetInstance()->GetServerSetting(Net::RemoteType::RT_GAME);
		m_HttpGame = new HttpGame();
		m_HttpGame->Start(info->GetHttpPort());		
		return true;
	}

	void ServerGame::OnUpdate()
	{
		for (ModuleVector::iterator itr = m_UpdateModules.begin(); itr != m_UpdateModules.end(); ++itr)
		{
			(*itr)->OnUpdate();
		}
	}

	void ServerGame::OnUpdatePerSecond()
	{
		//HTTP服务每秒派发一次的频率即可
		if (m_HttpGame != NULL)
		{
			m_HttpGame->Dispatch();
		}

		for (ModuleVector::iterator itr = m_Modules.begin(); itr != m_Modules.end(); ++itr)
		{
			(*itr)->OnUpdatePerSecond();
		}
	}

	void ServerGame::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
	}

	void ServerGame::InitModules()
	{
		m_Modules.push_back(new LoginModule());
		m_Modules.push_back(new PlayerModule());
	}

	bool ServerGame::OnReleaseStep(int step)
	{
		//停止Http服务
		if (step == 1)
		{
			m_HttpGame->Stop();
			SAFE_DELETE(m_HttpGame);
			return false;
		}

		//释放顺序应该逆着来
		int i = (int)m_Modules.size() - (step - 1);		//减1是因为step1已经用于释放Http服务了
		m_Modules[i]->Release();
		return i == 0;
	}

	void ServerGame::OnConnect(NetDataItem *item)
	{
		m_LoginModule->OnConnect(item);
	}

	void ServerGame::OnDisconnect(NetDataItem *item)
	{
		m_LoginModule->OnDisconnect(item);
	}
}