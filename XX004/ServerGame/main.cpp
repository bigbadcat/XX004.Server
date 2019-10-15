/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 游戏服程序入口。
*
* Author: XuXiang
* Date: 2019-10-02 10:48
*******************************************************/

#include <vld.h>
#include <iostream>
#include "MainGame.h"
using namespace std;


int main(int argc, char *argv[])
{
	int ret = 0;
	{
		int sid = XX004::MainBase::ParseServerID(argc, argv);
		if (sid > 0)
		{
			XX004::MainGame server;
			ret = server.Run(XX004::Net::RemoteType::RT_GAME, sid);
		}
		else
		{
			::printf_s("bad server id(%d).\n", sid);
		}
	}

 	system("pause");
	return ret;
}