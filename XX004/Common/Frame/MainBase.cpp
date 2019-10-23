/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainBase.cpp
* Summary: 程序入口管理的基类。
*
* Author: XuXiang
* Date: 2018-08-13 22:43
*******************************************************/

#include "MainBase.h"
#include "NetManagerBase.h"
#include "ServerBase.h"
#include "StorageManager.h"
#include "StartSetting.h"
#include "EventManager.h"
#include <iostream>
#include <assert.h>

namespace XX004
{
	MainBase* MainBase::pCurMain = NULL;

	const Int64 MainBase::FRAME_GAP = 100;

	const string MainBase::COMMAND_QUIT = "/q";

	MainBase::MainBase() : m_Type(0), m_pNetManager(NULL), m_pStorageManager(NULL), m_pServer(NULL)
	{
		pCurMain = this;
	}

	MainBase::~MainBase()
	{
		if (pCurMain == this)
		{
			pCurMain = NULL;
		}
		SAFE_DELETE(m_pStorageManager);
		SAFE_DELETE(m_pNetManager);
		SAFE_DELETE(m_pServer);
	}

	int MainBase::ParseServerID(int argc, char *argv[])
	{
		int sid = 0;
		if (argc < 2)
		{
			return sid;
		}

		sid = ::atoi(argv[1]);
		return sid;
	}

	int MainBase::Run(int type, int sid)
	{
		//初始化网络
		WORD wVersionRequested;
		WSADATA wsaData;
		int32_t err;
		wVersionRequested = MAKEWORD(2, 2);
		err = ::WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			printf_s("WSAStartup err:%d\n", err);
			return 1;
		}

		//模块创建		
		m_Type = type;
		m_pNetManager = OnCreateNetManager();
		assert(m_pNetManager != NULL);
		m_pServer = OnCreateServer();
		assert(m_pServer != NULL);
		m_pStorageManager = new StorageManager();
		m_pServer->RegisterStorageMessage(m_pStorageManager);
		m_pServer->RegisterNetMessage(m_pNetManager);

		//模块运行
		m_pStorageManager->Start(type, sid);
		ServerSetting* info = StartSetting::GetInstance()->GetServerSetting(m_Type);
		assert(info != NULL);
		m_pNetManager->Start(info->GetPort());
		m_pServer->Start(sid, true);
		CommandLoop();
		printf_s("Waitting end ...\n");
		m_pServer->Stop();
		m_pNetManager->Stop();
		m_pStorageManager->Stop();

		//模块销毁
		m_pNetManager->UnregisterAllCallBack();		//先清掉注册再删除Server
		m_pStorageManager->UnregisterAllCallBack();
		EventManager::GetInstance()->UnregisterAllCallBack();
		EventParam::ReleaseCaches();
		SAFE_DELETE(m_pServer);
		SAFE_DELETE(m_pNetManager);		
		SAFE_DELETE(m_pStorageManager);
		::WSACleanup();

		return 0;
	}

	void MainBase::CommandLoop()
	{
		printf_s("Start command ...\n");
		char str[64];
		while (true)
		{
			//接收命令
			cin.getline(str, sizeof(str));
			
			//如果输入内容超过缓冲区
			if (!std::cin)
			{
				std::cin.clear(); // 清除错误标志位
				std::cin.sync(); // 清除流
			}

			//提交命令
			string cmd(str);
			if (cmd.compare(COMMAND_QUIT) == 0)
			{
				break;
			}
			else
			{
				m_pServer->PostCommand(cmd);
			}			
		}
	}
}