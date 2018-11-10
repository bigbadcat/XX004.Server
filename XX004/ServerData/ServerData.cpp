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
#include "Util/TimeUtil.h"
#include "Net/NetMessage.h"
using namespace XX004::Net;

namespace XX004
{
	ServerData::ServerData()
	{
	}

	ServerData::~ServerData()
	{
	}

	void ServerData::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->RegisterMessageCallBack(1000, [this](NetDataItem *item){this->F1(item); });
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

	void ServerData::F1(NetDataItem *item)
	{
		NetMessageString nms;
		nms.Unpack(item->data, 0);
		cout << "text:" << nms.Value << endl;

		////准备回复数据		
		//NetMessageIntString res;
		//res.Value1 = 0;
		//res.Value2 = nms.Value + "hhhhh";

		//Byte recvdata[1024];
		//int index = 0;
		//index = res.Pack(recvdata, index);

		////回复的数据头
		//NetPackageHeader sendhead;
		//sendhead.SetSign();
		//sendhead.Command = 1050;
		//sendhead.BodySize = index;

		//Byte sendbuff[1024];
		//int sendsize = 0;
		//sendsize = sendhead.Pack(sendbuff + sendsize, sendsize);
		//::memcpy_s(sendbuff + sendsize, 1024 - sendsize, recvdata, index);
		//sendsize += index;

		//con->AddSendData(sendbuff, sendsize);
	}
}