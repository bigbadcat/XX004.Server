/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 游戏服程序入口。
*
* Author: XuXiang
* Date: 2019-10-02 10:48
*******************************************************/

#if defined(WIN)
#include <vld.h>
#endif
#include <iostream>
#include "Game/MainGame.h"
using namespace std;


int main(int argc, char *argv[])
{
	int ret = 0;
	{
		int sid = XX004::MainBase::ParseServerID(argc, argv);
		if (sid > 0)
		{
			XX004::MainGame server;
			ret = server.Run(sid);
		}
		else
		{
			::printf("bad server id(%d).\n", sid);
		}
	}

#if defined(WIN)
 	system("pause");
#endif
	return ret;
}