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
#include "../Util/TimeUtil.h"
#include "../Util/StringUtil.h"

namespace XX004
{
	bool CommandInfo::Init(const string& line)
	{
		cmd.clear();
		params.clear();

		//空格分隔
		static string SP(" ");
		vector<string> splist;
		bool iscmd = true;
		StringUtil::Split(line, SP, splist);
		for (vector<string>::iterator itr = splist.begin(); itr != splist.end(); ++itr)
		{
			string e = StringUtil::Trim(*itr);
			if (e.size() > 0)
			{
				if (iscmd)
				{
					cmd = e;
					iscmd = false;
				}
				else
				{
					params.push_back(e);
				}
			}
		}

		return IsValid();
	}

	std::ostream & operator<<(std::ostream &out, const CommandInfo &info)
	{
		out << info.cmd << ":";
		for (vector<string>::const_iterator itr = info.params.cbegin(); itr != info.params.cend(); ++itr)
		{
			if (itr != info.params.cbegin())
			{
				out << ",";
			}
			out << *itr;
		}
		return out;
	}

	const Int64 ServerBase::FRAME_GAP = 100;

	ServerBase::ServerBase() : m_State(ServerState::SS_CREATE), m_Group(0), m_ID(0)
	{
	}

	ServerBase::~ServerBase()
	{
		//不能直接销毁还没结束的服务
		assert(m_State == ServerState::SS_CREATE || m_State == ServerState::SS_END);
	}

	void ServerBase::Start(int id, bool wait_init_finish)
	{
		if (m_State != ServerState::SS_CREATE)
		{
			return;
		}
		m_Group = id / 1000;
		m_ID = id;
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
		JoinThread(m_Thread);
	}

	void ServerBase::PostCommand(const string& cmd_line)
	{
		CommandInfo *pinfo = new CommandInfo(cmd_line);
		if (pinfo->IsValid())
		{
			m_Cmds.Push(pinfo);
		}
		else
		{
			SAFE_DELETE(pinfo);
		}
	}

	void ServerBase::OnAddConfig(vector<ModuleConfig*> &cfgs)
	{
	}

	void ServerBase::OnCommand(const string& cmd, const vector<string> &param)
	{

	}

	void ServerBase::ThreadProcess()
	{
		//服务器完整周期 初始->更新->销毁
		m_State = ServerState::SS_INIT;
		Init();
		m_InitSemaphore.post();
		m_State = ServerState::SS_UPDATE;
		Update();			//Update函数会等m_State!=ServerState::SS_UPDATE时结束
		Release();
		m_State = ServerState::SS_END;
	}

	void ServerBase::HandleCommand()
	{
		//先进行不安全的次数只读判断
		if (m_Cmds.GetSize() <= 0)
		{
			return;
		}

		static queue<CommandInfo*> temp_queue;
		m_Cmds.MoveAll(temp_queue);

		//分发命令
		while (temp_queue.size() > 0)
		{
			CommandInfo *item = temp_queue.front();
			temp_queue.pop();
			OnCommand(item->cmd, item->params);
			SAFE_DELETE(item);
		}
	}

	void ServerBase::Init()
	{
		//分步初始化
		int step = 0;
		bool ok = false;
		int config_index = 0;
		m_Configs.clear();
		OnAddConfig(m_Configs);
		chrono::milliseconds dura(50);
		do
		{
			if (config_index < m_Configs.size())
			{
				ModuleConfig *cfg = m_Configs[config_index++];
				cfg->Init();
				printf_s("Init %s\n", cfg->GetName());
			}
			else
			{
				//先初始化配置
				ok = OnInitStep(++step);
				printf_s("Init step:%d\n", step);
			}

			this_thread::sleep_for(dura);
		} while (!ok);
		printf_s("Init complete\n");
	}

	void ServerBase::Update()
	{
		NetManagerBase *pNetMgr = MainBase::GetCurMain()->GetNetManager();
		UInt64 t_sectick = TimeUtil::GetTickCount();		//秒更新开始计数
		while (m_State == ServerState::SS_UPDATE)
		{
			UInt64 t_loopstart = TimeUtil::GetTickCount();		//循环开始的时间
			pNetMgr->Dispatch();
			HandleCommand();
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
				chrono::milliseconds dura(t_needsleep > 0 ? t_needsleep : 1);
				this_thread::sleep_for(dura);
				//cout << "t_needsleep:" << t_needsleep << endl;
			}
		}
	}

	void ServerBase::Release()
	{
		//分步释放
		int step = 0;
		bool ok = false;
		chrono::milliseconds dura(50);
		do
		{
			ok = OnReleaseStep(++step);
			printf_s("Release step:%d\n", step);
			this_thread::sleep_for(dura);
		} while (!ok);
		printf_s("Release complete\n");

		//销毁配置
		for (vector<ModuleConfig*>::iterator itr = m_Configs.begin(); itr != m_Configs.end(); ++itr)
		{
			(*itr)->Release();
		}
	}
}