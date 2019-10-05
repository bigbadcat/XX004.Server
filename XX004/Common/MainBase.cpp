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
#include "StorageBase.h"
#include "StartSetting.h"
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

	int MainBase::Run(int type)
	{
		//初始化网络
		WORD wVersionRequested;
		WSADATA wsaData;
		int32_t err;
		wVersionRequested = MAKEWORD(2, 2);
		err = ::WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			cout << "WSAStartup err:" << err << endl;
			return 1;
		}

		//模块创建		
		m_Type = type;
		m_pNetManager = OnCreateNetManager();
		assert(m_pNetManager != NULL);
		m_pServer = OnCreateServer();
		m_pStorageManager = new StorageBase();
		m_pServer->RegisterStorageMessage(m_pStorageManager);
		assert(m_pServer != NULL);
		m_pServer->RegisterNetMessage(m_pNetManager);

		//模块运行
		//int group = StartSetting::GetInstance()->GetGroup();
		//int id = StartSetting::GetInstance()->GetID();
		int id = 10001;		//通过命令行传入
		int group = id / 10000;
		StartSettingInfo* info = StartSetting::GetInstance()->GetSettingInfo(m_Type);
		assert(info != NULL);
		m_pStorageManager->Start();
		m_pNetManager->Start(info->GetPort());
		m_pServer->Start(group, id, true);
		CommandLoop();
		cout << "Waitting server end ..." << endl;
		m_pServer->Stop();
		m_pServer->Join();
		cout << "Waitting net end ..." << endl;
		m_pNetManager->Stop();
		m_pNetManager->Join();
		m_pStorageManager->Stop();

		//模块销毁
		m_pNetManager->UnregisterAllCallBack();		//先清掉注册再删除Server
		m_pStorageManager->UnregisterAllCallBack();
		SAFE_DELETE(m_pServer);
		SAFE_DELETE(m_pNetManager);		
		::WSACleanup();

		return 0;
	}

	void MainBase::CommandLoop()
	{
		cout << "Start command ..." << endl;
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