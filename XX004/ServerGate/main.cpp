/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 网关程序入口。
*
* Author: XuXiang
* Date: 2018-08-10 22:27
*******************************************************/

#include "Util/DataUtil.h"
#include "ServerMain.h"
#include <iostream>
#include <vld.h>
using namespace std;

class ServerMainGate : public XX004::ServerMain
{
public:
	ServerMainGate();
	~ServerMainGate();

private:

};

ServerMainGate::ServerMainGate()
{
}

ServerMainGate::~ServerMainGate()
{
}

int main(int argc, char *argv[])
{
	int ret = 0;
	{
		ServerMainGate server;
		ret = server.Run();
	}

	system("pause");
	return ret;
}