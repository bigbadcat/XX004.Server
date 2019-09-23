/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 场景服程序入口。
*
* Author: XuXiang
* Date: 2019-09-21 16:54
*******************************************************/

#include "MainScene.h"
#include <iostream>
#include <vld.h>
using namespace std;
using namespace XX004;


int main(int argc, char *argv[])
{
	int ret = 0;
	{
		XX004::MainScene server;
		ret = server.Run(XX004::Net::RemoteType::RT_SCENE);
	}

 	system("pause");
	return ret;
}