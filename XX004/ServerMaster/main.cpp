/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 中控程序入口。
*
* Author: XuXiang
* Date: 2019-09-25 16:02
*******************************************************/

#if defined(WIN)
#include <vld.h>
#endif
#include "Master/MainMaster.h"
#include <Util/FunctionUtil.h>

int main(int argc, char *argv[])
{
	XX004::FunctionUtil::StartDaemon();
	int ret = 0;
	{
		XX004::MainMaster server;
		ret = server.Run();
	}

#if defined(WIN)
	system("pause");
#endif
	return ret;
}