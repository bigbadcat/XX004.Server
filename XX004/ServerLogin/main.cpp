/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 网关程序入口。
*
* Author: XuXiang
* Date: 2018-12-18 19:56
*******************************************************/

#include "MainLogin.h"
#include <iostream>
#include <vld.h>
using namespace std;
using namespace XX004;


int main(int argc, char *argv[])
{
	int ret = 0;
	{
		XX004::MainLogin server;
		ret = server.Run(XX004::Net::RemoteType::RT_LOGIN);
	}

 	system("pause");
	return ret;
}