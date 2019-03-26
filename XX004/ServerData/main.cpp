/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 数据服程序入口。

* Author: XuXiang
* Date: 2018-08-10 22:31
*******************************************************/

#include <iostream>
#include <string>
#include "MainData.h"
#include <vld.h>
using namespace std;

int main(int argc, char *argv[])
{
	int ret = 0;
	{
		XX004::MainData server;
		ret = server.Run(XX004::Net::RemoteType::RT_DATA);
	}

	system("pause");
	return ret;
}