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
#include <NetManagerBase.h>
#include <Protocol/NetProtocol.h>
#include <MainBase.h>
using namespace XX004::Net;

namespace XX004
{
	ServerGate::ServerGate()
	{
	}

	ServerGate::~ServerGate()
	{
	}

	//UIDToUserNameMap m_UIDToUserName;
	//UserNameToUIDMap m_UserNameToUID;

	void ServerGate::SetRelation(Int64 uid, const std::string& username)
	{
		//UserNameToUIDMap::iterator itr_username = m_UserNameToUID.find(username);
		//if (itr_username != m_UserNameToUID.end())
		//{
		//	if (itr_username->second != uid)
		//	{

		//	}
		//	else
		//	{
		//		//重复设置
		//		return;
		//	}
		//}
	}

	void ServerGate::RemoveRelation(Int64 uid)
	{
		UIDToUserNameMap::iterator itr = m_UIDToUserName.find(uid);
		if (itr != m_UIDToUserName.end())
		{
			m_UserNameToUID.erase(itr->second);
			m_UIDToUserName.erase(uid);
		}
	}

	void ServerGate::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		pMgr->RegisterMessageCallBack(NetMsgID::CG_LOGIN_REQ, [this](NetDataItem *item){this->OnLoginRequest(item); });
		//pMgr->RegisterMessageCallBack(1000, [this](NetDataItem *item){this->F1(item); });
		//pMgr->RegisterMessageCallBack(1050, [this](NetDataItem *item){this->F2(item); });
	}

	bool ServerGate::OnInitStep(int step, float &r)
	{
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
		//if (cmd.compare("c1") == 0)
		//{
		//	NetMessageIntString req;
		//	req.Value1 = 0;
		//	req.Value2 = "xxxxxxxx";
		//	MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 1), 1000, &req);
		//}
	}

	bool ServerGate::OnReleaseStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerGate::OnConnect(NetDataItem *item)
	{
		//cout << "ServerGate::OnConnect uid:" << item->uid << " key:" << item->key << endl;
	}

	void ServerGate::OnDisconnect(NetDataItem *item)
	{
		//cout << "ServerGate::OnDisconnect uid:" << item->uid << " key:" << item->key << endl;
		RemoveRelation(item->uid);
	}

	void ServerGate::OnLoginRequest(NetDataItem *item)
	{
		CGLoginRequest req;
		req.Unpack(item->data, 0);
		cout << "OnLoginRequest username:" << req.UserName << endl;
		SetRelation(item->uid, req.UserName);
	}

	//void ServerGate::F1(NetDataItem *item)
	//{
	//	NetMessageIntString req;
	//	req.Unpack(item->data, 0);
	//	cout << "index:" << req.Value1 << " text:" << req.Value2 << endl;

	//	//准备回复数据		
	//	NetMessageIntString res;
	//	res.Value1 = 0;
	//	res.Value2 = req.Value2 + "hhhhh";

	//	Byte recvdata[1024];
	//	int index = 0;
	//	index = res.Pack(recvdata, index);

	//	NetManagerBase *pNetMgr = MainBase::GetCurMain()->GetNetManager();
	//	if (item->key.first == RemoteType::RT_UNKNOW)
	//	{
	//		pNetMgr->Update(item->uid, RemoteKey(RemoteType::RT_CLIENT, 1));
	//	}
	//	
	//	pNetMgr->Send(RemoteKey(RemoteType::RT_CLIENT, 1), 1050, recvdata, index);
	//	//pNetMgr->Close(RemoteKey(RemoteType::RT_CLIENT, 1));
	//}

	//void ServerGate::F2(NetDataItem *item)
	//{
	//	NetMessageIntString req;
	//	req.Unpack(item->data, 0);
	//	cout << "result:" << req.Value1 << " text:" << req.Value2 << endl;
	//}
}