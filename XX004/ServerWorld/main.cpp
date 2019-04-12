/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 世界服程序入口。
*
* Author: XuXiang
* Date: 2019-04-12 16:33
*******************************************************/

#include <vld.h>
#include "MainWorld.h"

int main(int argc, char *argv[])
{
	int ret = 0;
	{
		XX004::MainWorld server;
		ret = server.Run(XX004::Net::RemoteType::RT_WORLD);
	}

 	system("pause");
	return ret;
}