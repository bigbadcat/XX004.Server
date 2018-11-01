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
		m_IsRunning = true;
		m_pNetManager = OnCreateNetManager();
		assert(m_pNetManager != NULL);
		m_pServer = OnCreateServer();
		assert(m_pServer != NULL);
		m_pServer->RegisterNetMessage(m_pNetManager);

		//服务器中
		SeverLoop();

		//停止并销毁
	}

	void ServerMain::SeverLoop()
	{
		int state = 0;				//状态 0初始化 1更新 2停止 3跳出
		int startstep = 0, stopstep = 0;
		bool loop = true;
		UInt64 t_loopstart = 0;		//循环开始的时间
		UInt64 t_sectick = 0;		//秒更新开始计数
		while (loop)
		{
			t_loopstart = TimeUtil::GetTickCount();
			switch (state)
			{
				case 0:
					if (StarStep(++startstep))
					{
						state = 1;
						t_sectick = TimeUtil::GetTickCount();
					}
					break;
				case 1:
					if (UpdateStep(t_sectick))
					{
						state = 2;
					}
					break;
				case 2:
					if (StarStep(++stopstep))
					{
						state = 3;
						loop = false;
					}
					break;
				default:
					break;
			}

			//帧停留
			UInt64 t_loopend = TimeUtil::GetTickCount();
			if (t_loopend > t_loopstart)
			{
				UInt64 t_needsleep = FRAME_GAP - (t_loopend - t_loopstart);
				::Sleep(t_needsleep > 0 ? t_needsleep : 1);
				//cout << "t_needsleep:" << t_needsleep << endl;
			}
		}
	}

	bool ServerMain::StarStep(int step)
	{
		float r = 0;
		bool ok = m_pServer->OnStart(step, r);
		MoveCursorBack(24);
		cout << "Start step:" << step << " r:" << (int)(r * 100) << "%";
		if (ok)
		{
			MoveCursorBack(24);
			cout << endl << "Start complete" << endl;
			m_StartSemaphore.post();
			return true;
		}
		return false;
	}

	bool ServerMain::UpdateStep(UInt64 &sectick)
	{
		//处理网络消息、模块更新
		m_pNetManager->OnUpdate();
		m_pServer->OnUpdate();
		if (TimeUtil::GetTickCount() - sectick >= 1000)
		{
			sectick += 1000;
			m_pServer->OnUpdatePerSecond();
		}

		if (!m_IsRunning)
		{
			return true;
		}
		return false;
	}

	bool ServerMain::StopStep(int step)
	{
		float r = 0;
		bool ok = m_pServer->OnStop(step, r);
		MoveCursorBack(24);
		cout << "Stop Step:" << step << " r:" << (int)(r * 100) << "%";
		if (ok)
		{
			MoveCursorBack(24);
			cout << endl << "Stop complete" << endl;
			return true;
		}
		return false;
	}
}