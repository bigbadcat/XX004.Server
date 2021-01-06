/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: FunctionUtil.cpp
* Summary: 通用操作函数。
*
* Author: XuXiang
* Date: 2021-01-06 21:26
*******************************************************/

#include "FunctionUtil.h"
#if !defined(WIN)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <csignal>
#endif

namespace XX004
{
	bool is_run = false;

	void OnSignal(int signum)
	{
		printf("OnSignal %d\n", signum);
		is_run = false;
	}

	void FunctionUtil::CommandLoop(CommandCallBack call)
	{
#if defined(WIN)
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
			if (cmd.compare("/q") == 0)
			{
				break;
			}
			else if (call)
			{
				call(cmd);
			}
		}
#else
		//linux下走信号退出
		is_run = true;
		signal(SIGTERM, OnSignal);
		signal(SIGINT, OnSignal);
		std::chrono::milliseconds dura(200);
		while (is_run)
		{
			std::this_thread::sleep_for(dura);
		}
#endif
	}

}
