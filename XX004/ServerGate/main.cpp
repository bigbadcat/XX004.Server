/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 网关程序入口。
*
* Author: XuXiang
* Date: 2018-08-10 22:27
*******************************************************/

#include "MainGate.h"
#include <iostream>
#include <vld.h>
using namespace std;


int main(int argc, char *argv[])
{
	int ret = 0;
	{
		XX004::MainGate server;
		ret = server.Run();
	}

	system("pause");
	return ret;
}