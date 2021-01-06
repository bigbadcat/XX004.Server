/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainMaster.cpp
* Summary: 中控服程序入口管理。
*
* Author: XuXiang
* Date: 2019-09-27 17:07
*******************************************************/

#include "MainMaster.h"
#include "ServerMaster.h"
#include <Frame/StartSetting.h>
#if defined(WIN)
#include <WinSock2.h>
#endif
#include <iostream>
#include <Util/FunctionUtil.h>
using namespace std;

namespace XX004
{
	MainMaster::MainMaster()
	{		
	}

	MainMaster::~MainMaster()
	{		
	}

	int MainMaster::Run()
	{
#if defined(WIN)
		//初始化网络
		WORD wVersionRequested;
		WSADATA wsaData;
		int32_t err;
		wVersionRequested = MAKEWORD(2, 2);
		err = ::WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			printf_s("WSAStartup err:%d\n", err);
			return 1;
		}

#endif

		ServerMaster server;
		short port = StartSetting::GetInstance()->GetMasterPort();
		server.Start(port);

		//等待命令结束
		CommandCallBack on_cmd = [&server](const string& cmd)
		{
			if (cmd.compare("/r") == 0)
			{
				server.SetServerDirty();
			}
		};
		FunctionUtil::CommandLoop(on_cmd);
		server.Stop();

#if defined(WIN)
		::WSACleanup();
#endif
		return 0;
	}
}