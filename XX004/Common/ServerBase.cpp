/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerBase.cpp
* Summary: 服务模块的基类。
*
* Author: XuXiang
* Date: 2018-10-25 19:24
*******************************************************/

#include "ServerBase.h"
#include "MainBase.h"
#include <assert.h>
#include "Util/TimeUtil.h"

namespace XX004
{
	const Int64 ServerBase::FRAME_GAP = 100;

	ServerBase::ServerBase() : m_State(ServerState::SS_CREATE)
	{
	}

	ServerBase::~ServerBase()
	{
		//不能直接销毁还没结束的服务
		assert(m_State == ServerState::SS_CREATE || m_State == ServerState::SS_END);
	}

	void ServerBase::Start(bool wait_init_finish)
	{
		if (m_State != ServerState::SS_CREATE)
		{
			return;
		}
		m_Thread = thread([](ServerBase *t){t->ThreadProcess(); }, this);

		//初始完毕
		m_State = ServerState::SS_UPDATE;
		if (wait_init_finish)
		{
			m_InitSemaphore.wait();
		}
	}

	void ServerBase::Stop()
	{
		//分步释放
		if (m_State != ServerState::SS_UPDATE)
		{
			return;
		}
		m_State = ServerState::SS_RELEASE;
	}

	void ServerBase::Join()
	{
		JoinThread(m_Thread);
	}

	void ServerBase::ThreadProcess()
	{
		//服务器完整周期 初始->更新->销毁
		m_State = ServerState::SS_INIT;
		Init();
		m_InitSemaphore.post();
		m_State = ServerState::SS_UPDATE;
		Update();			//Update函数会等m_State==ServerState::SS_RELEASE时结束
		Release();
		m_State = ServerState::SS_END;
	}

	void ServerBase::Init()
	{
		//分步初始化
		float r = 0;
		int step = 0;
		bool ok = false;
		std::chrono::milliseconds dura(50);
		do
		{
			ok = OnInitStep(++step, r);
			MoveCursorBack(24);
			cout << "Init step:" << step << " r:" << (int)(r * 100) << "%";
			std::this_thread::sleep_for(dura);
		} while (!ok);
		MoveCursorBack(24);
		cout << endl << "Init complete" << endl;
	}

	void ServerBase::Update()
	{
		NetManagerBase *pNetMgr = MainBase::GetCurMain()->GetNetManager();
		UInt64 t_sectick = TimeUtil::GetTickCount();		//秒更新开始计数
		while (m_State == ServerState::SS_UPDATE)
		{
			UInt64 t_loopstart = TimeUtil::GetTickCount();		//循环开始的时间
			OnUpdate();
			if (t_loopstart - t_sectick >= 1000)
			{
				t_sectick += 1000;
				OnUpdatePerSecond();
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

	void ServerBase::Release()
	{
		//分步释放
		float r = 0;
		int step = 0;
		bool ok = false;
		std::chrono::milliseconds dura(50);
		do
		{
			ok = OnReleaseStep(++step, r);
			MoveCursorBack(24);
			cout << "Release step:" << step << " r:" << (int)(r * 100) << "%";
			std::this_thread::sleep_for(dura);
		} while (!ok);
		MoveCursorBack(24);
		cout << endl << "Release complete" << endl;
	}
}