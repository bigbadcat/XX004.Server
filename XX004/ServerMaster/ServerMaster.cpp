/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerMaster.cpp
* Summary: 中控服务。
*
* Author: XuXiang
* Date: 2019-09-27 17:25
*******************************************************/

#include "ServerMaster.h"
#include <iostream>
using namespace std;

namespace XX004
{
	ServerMaster::ServerMaster()
	{		
	}

	ServerMaster::~ServerMaster()
	{		
	}

	void ServerMaster::OnRegisterServer()
	{
		HTTP_SERVER_REGISTER("/login", OnLoginRequest);
	}

	void ServerMaster::OnInit()
	{

	}

	void ServerMaster::OnLoop()
	{
	}

	void ServerMaster::OnRelease()
	{
	}

	void ServerMaster::OnLoginRequest(const string &path, HttpParamMap &params, HttpResponse &res)
	{
		string user = params["user"];
		string pwd = params["pwd"];
		cout << "OnLoginRequest cmd:" << path << " user:" << user << " pwd:" << pwd << endl;

		res.AddText("Hello %s!", user.c_str());
	}
}