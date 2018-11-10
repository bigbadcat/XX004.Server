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
#include "MySQLTest.h"
#include "NetTest.h"
#include "MainData.h"
#include <vld.h>
using namespace std;

int main(int argc, char *argv[])
{
	//cout << "Hello SvrData!" << endl;
	//XX004::MySQLTest mysqltest;
	//mysqltest.DoTest();

	//XX004::NetTest nettest;
	//nettest.Run();

	//nettest.Stop();

	int ret = 0;
	{
		XX004::MainData server;
		ret = server.Run();
	}

	system("pause");
	return ret;
}