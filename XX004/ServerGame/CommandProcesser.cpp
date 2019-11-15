/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: CommandProcesser.cpp
* Summary: 命令处理。
*
* Author: XuXiang
* Date: 2019-11-15 17:27
*******************************************************/

#include "CommandProcesser.h"
#include "ServerGame.h"
#include "Module/Player/PlayerModule.h"
#include "Module/Player/PlayerBasicData.h"
#include <Frame/ModuleBase.h>
#include <Config/BasicModuleConfig.h>

namespace XX004
{
	CommandProcesser::CommandProcesser() : m_Server(NULL)
	{
	}

	CommandProcesser::~CommandProcesser()
	{
	}

	CommandProcesser* CommandProcesser::GetInstance()
	{
		static CommandProcesser cmd;
		return &cmd;
	}

	void CommandProcesser::Init(ServerGame *server)
	{
		m_Server = server;
	}

	void CommandProcesser::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
		if (cmd.compare("/addlevel") == 0)
		{
			int lv = param.size() > 0 ? ::atoi(param[0].c_str()) : 0;
			PlayerModule *model = m_Server->GetModule<PlayerModule>();
			PlayerBasicData *player = model->GetPlayer(10001000001);
			Int64 exp = ModuleConfig::GetInstance<BasicModuleConfig>()->GetNeedExp(player->GetLevel(), player->GetLevel() + lv);
			player->AddExp(exp - player->GetExp());
			player->RebuildAttr();
		}
		else if (cmd.compare("/addexp") == 0)
		{
			int exp = param.size() > 0 ? ::atoi(param[0].c_str()) : 0;
			PlayerModule *model = m_Server->GetModule<PlayerModule>();
			PlayerBasicData *player = model->GetPlayer(10001000001);
			player->AddExp(exp);
			player->RebuildAttr();
		}
	}
}