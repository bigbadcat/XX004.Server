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
#include "MainBase.h"
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
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		pMgr->RegisterMessageCallBack(1000, [this](NetDataItem *item){this->F1(item); });
	}

	bool ServerData::OnInitStep(int step, float &r)
	{
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

	bool ServerData::OnReleaseStep(int step, float &r)
	{
		//r = step / 50.0f;
		//return step >= 50;
		r = 1;
		return true;
	}

	void ServerData::OnConnect(NetDataItem *item)
	{
		cout << "ServerData::OnConnect uid:" << item->uid << endl;
	}

	void ServerData::OnDisconnect(NetDataItem *item)
	{
		cout << "ServerData::OnDisconnect uid:" << item->uid << " key:" << item->key << endl;
	}

	void ServerData::F1(NetDataItem *item)
	{
		NetMessageIntString req;
		req.Unpack(item->data, 0);
		cout << "index:" << req.Value1 << " text:" << req.Value2 << endl;

		//准备回复数据		
		NetMessageIntString res;
		res.Value1 = 0;
		res.Value2 = req.Value2 + "hhhhh";

		Byte recvdata[1024];
		int index = 0;
		index = res.Pack(recvdata, index);

		NetManagerBase *pNetMgr = MainBase::GetCurMain()->GetNetManager();
		pNetMgr->Update(item->uid, RemoteKey(RemoteType::RT_CLIENT, 1));
		pNetMgr->Send(item->uid, 1050, recvdata, index);
	}
}