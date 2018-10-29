/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerData.cpp
* Summary: 数据服务模块。
*
* Author: XuXiang
* Date: 2018-10-25 19:41
*******************************************************/

#include "ServerData.h"
#include "NetManagerData.h"
#include "ServerMain.h"
#include "Util/TimeUtil.h"

namespace XX004
{
	ServerData::ServerData()
	{
	}

	ServerData::~ServerData()
	{
	}

	void ServerData::RegisterNetMessage(NetManager *pMgr)
	{
		pMgr->RegisterMessageCallBack(1, [this](Int32 cmd, Byte *buffer, int len){this->F1(cmd, buffer, len); });
		pMgr->RegisterMessageCallBack(2, [this](Int32 cmd, Byte *buffer, int len){this->F2(cmd, buffer, len); });
	}

	bool ServerData::OnStart(int step, float &r)
	{
		//ServerMain::GetCurMain()->GetNetManager()->Test(2);
		//r = step / 80.0f;
		//return step >= 80;
		r = 1;
		return true;
	}

	void ServerData::OnUpdate()
	{
		//cout << "ServerData::OnUpdate() " << TimeUtil::GetCurrentMillisecond() << endl;
	}

	void ServerData::OnUpdatePerSecond()
	{
		//cout << "ServerData::OnUpdatePerSecond()" << TimeUtil::GetCurrentMillisecond() << endl;
	}

	bool ServerData::OnStop(int step, float &r)
	{
		//r = step / 50.0f;
		//return step >= 50;

		r = 1;
		return true;
	}

	void ServerData::F1(Int32 cmd, Byte *buffer, int len)
	{
		cout << "F1" << endl;
	}

	void ServerData::F2(Int32 cmd, Byte *buffer, int len)
	{
		cout << "F2" << endl;
	}
}