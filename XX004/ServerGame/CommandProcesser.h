/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: CommandProcesser.h
* Summary: 命令处理。
*
* Author: XuXiang
* Date: 2019-11-15 17:27
*******************************************************/

#ifndef __CommandProcesser_h__
#define __CommandProcesser_h__

#include <string>
#include <vector>
using namespace std;

namespace XX004
{
	class ServerGame;
	class CommandProcesser
	{
	public:
		
		static CommandProcesser* GetInstance();

		void Init(ServerGame *server);

		void OnCommand(const std::string& cmd, const std::vector<std::string> &param);

	private:

		CommandProcesser();
		~CommandProcesser();

		ServerGame *m_Server;
	};
}

#endif