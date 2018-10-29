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
#include "Util/TimeUtil.h"
#include "NetManager.h"
#include "ServerBase.h"
#include <iostream>
#include <assert.h>

namespace XX004
{
	ServerMain* ServerMain::pCurMain = NULL;

	const Int64 ServerMain::FRAME_GAP = 100;

	const string ServerMain::COMMAND_QUIT = "/q";

	ServerMain::ServerMain() : m_pNetManager(NULL), m_pServer(NULL), m_IsRunning(false)
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
		//启动服务循环
		m_Thread = thread([](ServerMain *t){t->ThreadProcess(); }, this);

		//等待初始化完毕的信号后开始命令循环
		m_StartSemaphore.wait();
		cout << "Start command ..." << endl;
		while (true)
		{
			string cmd;
			cin >> cmd;
			if (cmd.compare(COMMAND_QUIT) == 0)
			{
				m_IsRunning = false;
				break;
			}
			cout << "Unknow command:" << cmd << endl;
		}

		cout << "Waitting server end ..." << endl;
		JoinThread(m_Thread);

		return 0;
	}

	void ServerMain::ThreadProcess()
	{
		//模块创建
		int state = 0;				//状态 0初始化 1更新 2停止 3跳出
		m_IsRunning = true;
		m_pNetManager = OnCreateNetManager();
		assert(m_pNetManager != NULL);
		m_pServer = OnCreateServer();
		assert(m_pServer != NULL);
		m_pServer->RegisterNetMessage(m_pNetManager);

		int step = 0;
		float r = 0;
		UInt64 t_loopstart = 0;		//循环开始的时间
		UInt64 t_sectick = 0;		//秒更新开始计数
		while (true)
		{
			t_loopstart = TimeUtil::GetTickCount();
			if (state == 0)
			{
				bool ok = m_pServer->OnStart(++step, r);
				MoveCursorBack(24);
				cout << "Start step:" << step << " r:" << (int)(r*100) << "%";
				if (ok)
				{
					MoveCursorBack(24);
					cout << endl << "Start complete" << endl;
					state = 1;
					step = 0;
					r = 0;
					t_sectick = TimeUtil::GetTickCount();
					m_StartSemaphore.post();
				}
			}
			else if (state == 1)
			{
				//处理网络消息、模块更新
				m_pNetManager->OnUpdate();
				m_pServer->OnUpdate();
				if (TimeUtil::GetTickCount() - t_sectick >= 1000)
				{
					t_sectick += 1000;
					m_pServer->OnUpdatePerSecond();
				}

				if (!m_IsRunning)
				{
					state = 2;
					step = 0;
					r = 0;
				}
			}
			else if (state == 2)
			{
				bool ok = m_pServer->OnStop(++step, r);
				MoveCursorBack(24);
				cout << "Stop Step:" << step << " r:" << (int)(r * 100) << "%";
				if (ok)
				{
					MoveCursorBack(24);
					cout << endl << "Stop complete" << endl;
					state = 3;
					break;
				}
			}

			//帧停留
			UInt64 t_loopend = TimeUtil::GetTickCount();
			if (t_loopend > t_loopstart)
			{
				UInt64 t_needsleep = FRAME_GAP - (t_loopend - t_loopstart);
				if (t_needsleep > 0)
				{
					::Sleep(t_needsleep);
					//cout << "t_needsleep:" << t_needsleep << endl;
				}
			}						
		}
	}
}