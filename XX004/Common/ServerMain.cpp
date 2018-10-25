/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerMain.cpp
* Summary: 程序入口管理的基类。
*
* Author: XuXiang
* Date: 2018-08-13 22:43
*******************************************************/

#include "ServerMain.h"
#include "Util/DataUtil.h"
#include "NetManager.h"
#include "ServerBase.h"
#include <iostream>
#include <assert.h>

namespace XX004
{
	ServerMain* ServerMain::pCurMain = NULL;

	ServerMain::ServerMain() : m_pNetManager(NULL), m_pServer(NULL)
	{
		pCurMain = this;
	}

	ServerMain::~ServerMain()
	{
		if (pCurMain == this)
		{
			pCurMain = NULL;
		}
		SAFE_DELETE(m_pNetManager);
		SAFE_DELETE(m_pServer);
	}

	int ServerMain::Run()
	{
		//模块创建
		m_pNetManager = OnCreateNetManager();
		assert(m_pNetManager != NULL);
		m_pServer = OnCreateServer();
		assert(m_pServer != NULL);
		m_pServer->RegisterNetMessage(m_pNetManager);

		//初始化
		float r = 0;
		m_pServer->OnStart(0, r);

		//逻辑循环

		//停止
		m_pServer->OnStop(0, r);

		//double v = 51238956461.264590;
		//Byte buff[64];
		//::memset(buff, 0, sizeof(buff));
		//int wpos = 0;
		//wpos = DataUtil::WriteDouble(buff, 0, v);
		//int rpos = 0;
		//double r = DataUtil::ReadDouble(buff, rpos, &rpos);

		//std::string v = "sdf123换行e4";
		////std::string v = "";
		////std::string v = "123";
		////std::string v = "换行";
		//Byte buff[64];
		//::memset(buff, 0xFF, sizeof(buff));
		//int wpos = 0;
		//wpos = DataUtil::WriteString(buff, 0, v, 8);
		//int rpos = 0;
		//std::string r = DataUtil::ReadString(buff, rpos, &rpos, 8);


		//m_pNetManager->RegisterMessageCallBack(1, [this](Int32 cmd, Byte *buffer, int len){this->F1(cmd, buffer, len); });
		//m_pNetManager->RegisterMessageCallBack(2, [this](Int32 cmd, Byte *buffer, int len){this->F2(cmd, buffer, len); });
		//REGISTER_MESSAGE_CALLBACK(1, F1);
		//REGISTER_MESSAGE_CALLBACK(2, F2);

		//m_pNetManager->Test(2);

		return 0;
	}
}