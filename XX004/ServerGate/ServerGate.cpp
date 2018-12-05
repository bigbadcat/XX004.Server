/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerGate.cpp
* Summary: 网关服务模块。
*
* Author: XuXiang
* Date: 2018-12-03 19:42
*******************************************************/

#include "ServerGate.h"
#include "NetManagerBase.h"
#include "Util/TimeUtil.h"
#include "Net/NetMessage.h"
#include "MainBase.h"
using namespace XX004::Net;

namespace XX004
{
	ServerGate::ServerGate()
	{
	}

	ServerGate::~ServerGate()
	{
	}

	void ServerGate::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		pMgr->RegisterMessageCallBack(1000, [this](NetDataItem *item){this->F1(item); });
		pMgr->RegisterMessageCallBack(1050, [this](NetDataItem *item){this->F2(item); });
	}

	bool ServerGate::OnInitStep(int step, float &r)
	{
		//r = step / 80.0f;
		//return step >= 80;
		r = 1;
		return true;
	}

	void ServerGate::OnUpdate()
	{
		//cout << "ServerGate::OnUpdate() " << TimeUtil::GetCurrentMillisecond() << endl;
	}

	void ServerGate::OnUpdatePerSecond()
	{
		//cout << "ServerGate::OnUpdatePerSecond()" << TimeUtil::GetCurrentMillisecond() << endl;
	}

	void ServerGate::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
		if (cmd.compare("c1") == 0)
		{
			NetMessageIntString req;
			req.Value1 = 0;
			req.Value2 = "xxxxxxxx";
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 1), 1000, &req);
		}
	}

	bool ServerGate::OnReleaseStep(int step, float &r)
	{
		//r = step / 50.0f;
		//return step >= 50;
		r = 1;
		return true;
	}

	void ServerGate::OnConnect(NetDataItem *item)
	{
		cout << "ServerGate::OnConnect uid:" << item->uid << " key:" << item->key << endl;
	}

	void ServerGate::OnDisconnect(NetDataItem *item)
	{
		cout << "ServerGate::OnDisconnect uid:" << item->uid << " key:" << item->key << endl;
	}

	void ServerGate::F1(NetDataItem *item)
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
		if (item->key.first == RemoteType::RT_UNKNOW)
		{
			pNetMgr->Update(item->uid, RemoteKey(RemoteType::RT_CLIENT, 1));
		}
		
		pNetMgr->Send(RemoteKey(RemoteType::RT_CLIENT, 1), 1050, recvdata, index);
		//pNetMgr->Close(RemoteKey(RemoteType::RT_CLIENT, 1));
	}

	void ServerGate::F2(NetDataItem *item)
	{
		NetMessageIntString req;
		req.Unpack(item->data, 0);
		cout << "result:" << req.Value1 << " text:" << req.Value2 << endl;
	}
}