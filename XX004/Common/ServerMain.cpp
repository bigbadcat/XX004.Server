/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerMain.cpp
* Summary: 程序入口管理的基类。
*
* Author: XuXiang
* Date: 2018-08-13 22:43
*******************************************************/

#include "ServerMain.h"
#include "Util/DataUtil.h"
#include <iostream>

namespace XX004
{
	ServerMain::ServerMain()
	{

	}

	int ServerMain::Run()
	{
		//double v = 51238956461.264590;
		//Byte buff[64];
		//::memset(buff, 0, sizeof(buff));
		//int wpos = 0;
		//wpos = DataUtil::WriteDouble(buff, 0, v);
		//int rpos = 0;
		//double r = DataUtil::ReadDouble(buff, rpos, &rpos);

		std::string v = "sdf123换行e4";
		//std::string v = "";
		//std::string v = "123";
		//std::string v = "换行";
		Byte buff[64];
		::memset(buff, 0xFF, sizeof(buff));
		int wpos = 0;
		wpos = DataUtil::WriteString(buff, 0, v, 8);
		int rpos = 0;
		std::string r = DataUtil::ReadString(buff, rpos, &rpos, 8);

		return 0;
	}
}