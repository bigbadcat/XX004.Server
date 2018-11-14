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
#include "Util/TimeUtil.h"
#include "NetManagerBase.h"
#include "ServerBase.h"
#include <iostream>
#include <assert.h>
#include <chrono>

namespace XX004
{
	MainBase* MainBase::pCurMain = NULL;

	const Int64 MainBase::FRAME_GAP = 100;

	const string MainBase::COMMAND_QUIT = "/q";

	MainBase::MainBase() : m_pNetManager(NULL), m_pServer(NULL), m_IsRunning(false)
	{
		pCurMain = this;
	}

	MainBase::~MainBase()
	{
		if (pCurMain == this)
		{
			pCurMain = NULL;
		}
		SAFE_DELETE(m_pNetManager);
		SAFE_DELETE(m_pServer);
	}

	int MainBase::Run()
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

		////启动服务循环
		//m_Thread = thread([](MainBase *t){t->ThreadProcess(); }, this);

		////等待初始化完毕的信号后开始命令循环
		//m_StartSemaphore.wait();
		//cout << "Start command ..." << endl;
		//while (true)
		//{
		//	string cmd;
		//	cin >> cmd;
		//	if (cmd.compare(COMMAND_QUIT) == 0)
		//	{
		//		m_IsRunning = false;
		//		break;
		//	}
		//	cout << "Unknow command:" << cmd << endl;
		//}

		//cout << "Waitting server end ..." << endl;
		//JoinThread(m_Thread);

		//模块创建		
		m_pNetManager = OnCreateNetManager();
		assert(m_pNetManager != NULL);
		m_pServer = OnCreateServer();
		assert(m_pServer != NULL);
		m_pServer->RegisterNetMessage(m_pNetManager);

		m_pNetManager->Start();
		m_pServer->Start(true);
		CommandLoop();
		cout << "Waitting server end ..." << endl;
		m_pServer->Stop();		//Server停止还需要依赖网络，如逻辑服停止后需要把服务器状态数据提交给数据服
		m_pServer->Join();
		cout << "Waitting net end ..." << endl;
		m_pNetManager->Stop();
		m_pNetManager->Join();

		//模块销毁
		SAFE_DELETE(m_pNetManager);
		SAFE_DELETE(m_pServer);
		::WSACleanup();

		return 0;
	}

	void MainBase::ThreadProcess()
	{
		

		//模块创建		
		m_IsRunning = true;
		m_pNetManager = OnCreateNetManager();
		assert(m_pNetManager != NULL);
		m_pServer = OnCreateServer();
		assert(m_pServer != NULL);
		m_pServer->RegisterNetMessage(m_pNetManager);
		//m_pNetManager->Start("127.0.0.1", 9000);

		//服务器中
		SeverLoop();

		//停止并销毁
		m_pNetManager->Stop();
		SAFE_DELETE(m_pNetManager);
		SAFE_DELETE(m_pServer);		
	}

	void MainBase::SeverLoop()
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
				std::chrono::milliseconds dura(t_needsleep > 0 ? t_needsleep : 1);
				std::this_thread::sleep_for(dura);
				//cout << "t_needsleep:" << t_needsleep << endl;
			}
		}
	}

	bool MainBase::StarStep(int step)
	{
		//float r = 0;
		//bool ok = m_pServer->OnStart(step, r);
		//MoveCursorBack(24);
		//cout << "Start step:" << step << " r:" << (int)(r * 100) << "%";
		//if (ok)
		//{
		//	MoveCursorBack(24);
		//	cout << endl << "Start complete" << endl;
		//	m_StartSemaphore.post();
		//	return true;
		//}
		//return false;
		return true;
	}

	bool MainBase::UpdateStep(UInt64 &sectick)
	{
		////处理网络消息、模块更新
		//m_pNetManager->OnUpdate();
		//m_pServer->OnUpdate();
		//if (TimeUtil::GetTickCount() - sectick >= 1000)
		//{
		//	sectick += 1000;
		//	m_pServer->OnUpdatePerSecond();
		//}

		if (!m_IsRunning)
		{
			return true;
		}
		return false;
	}

	bool MainBase::StopStep(int step)
	{
		//float r = 0;
		//bool ok = m_pServer->OnStop(step, r);
		//MoveCursorBack(24);
		//cout << "Stop Step:" << step << " r:" << (int)(r * 100) << "%";
		//if (ok)
		//{
		//	MoveCursorBack(24);
		//	cout << endl << "Stop complete" << endl;
		//	return true;
		//}
		//return false;
		return true;
	}

	void MainBase::CommandLoop()
	{
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
	}
}