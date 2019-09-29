﻿/*******************************************************
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
#include <WinSock2.h>
#include <iostream>
using namespace std;

namespace XX004
{
	MainMaster::MainMaster()
	{		
	}

	MainMaster::~MainMaster()
	{		
	}

	void wait_quit()
	{
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
		}
	}

	int MainMaster::Run()
	{
		//初始化网络
		WORD wVersionRequested;
		WSADATA wsaData;
		int32_t err;
		wVersionRequested = MAKEWORD(2, 2);
		err = ::WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			::printf_s("WSAStartup err:%d\n", err);
			return 1;
		}

		ServerMaster server;
		server.Start(8081);
		wait_quit();
		server.Stop();

		::WSACleanup();
		return 0;
	}
}