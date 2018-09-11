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
using namespace std;

const char *COMMAND_QUIT = "quit";

int main(int argc, char *argv[])
{
	//cout << "Hello SvrData!" << endl;
	//XX004::MySQLTest mysqltest;
	//mysqltest.DoTest();

	XX004::NetTest nettest;
	nettest.Run();

	//命令循环
	//cout << "Start command ..." << endl;
	while (true)
	{
		string cmd;
		cin >> cmd;
		if (cmd.compare(COMMAND_QUIT) == 0)
		{
			break;
		}
		cout << "Unknow command:" << cmd << endl;
	}
	nettest.Stop();

	system("pause");
	return 0;
}