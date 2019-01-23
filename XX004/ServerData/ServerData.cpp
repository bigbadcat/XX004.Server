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
#include "NetManagerBase.h"
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
		pMgr->RegisterMessageCallBack(1050, [this](NetDataItem *item){this->F2(item); });
	}

	bool ServerData::OnInitStep(int step, float &r)
	{
		m_MySQL.Init();

		//char sql[128];
		//{
		//	sprintf_s(sql, "call sp_select_role(%I64d);", 214748364734);
		//	auto ret = m_MySQL.Query(sql);
		//	while (ret->GetRecord())
		//	{
		//		__int64 id = ret->GetInt64("id");
		//		const char *name = ret->GetString("name");
		//		int level = ret->GetInt("level");
		//		int exp = ret->GetInt("exp");
		//		int money = ret->GetInt("money");
		//		__int64 create_time = ret->GetInt64("create_time");
		//		cout << "id:" << id << " name:" << name << " level:" << level << " exp:" << exp << " money:" << money << " create_time:" << create_time << endl;
		//	}
		//}

		//{
		//	sprintf_s(sql, "call sp_select_role_skill(%I64d);", 214748364734);
		//	auto ret = m_MySQL.Query(sql);
		//	while (ret->GetRecord())
		//	{
		//		__int64 id = ret->GetInt64("role_id");
		//		int skill = ret->GetInt("skill_id");
		//		int level = ret->GetInt("level");
		//		cout << "role:" << id << " skill:" << skill << " level:" << level << endl;
		//	}
		//}

		//bool ok = true;
		//m_MySQL.StartTransaction();
		//do
		//{
		//	{
		//		//保存玩家数据
		//		__int64 id = 214748364734;
		//		string name = "kkk";
		//		int level = 10;
		//		int exp = 300;
		//		int money = 1000;
		//		__int64 create_time = 14214748364734;
		//		sprintf_s(sql, "call sp_insert_update_role(%I64d,'%s',%d,%d,%d,%I64d);", id, name.c_str(), level, exp, money, create_time);
		//		if (!m_MySQL.Execute(sql))
		//		{
		//			ok = false;
		//			break;
		//		}
		//	}

		//	{
		//		//保存技能数据
		//		__int64 id = 214748364734;
		//		int skill = 2001;
		//		int level = 15;
		//		__int64 create_time = 14214748364734;
		//		sprintf_s(sql, "call sp_insert_update_skill(%I64d,%d,%d);", id, skill, level);
		//		if (!m_MySQL.Execute(sql))
		//		{
		//			ok = false;
		//			break;
		//		}
		//	}
		//} while (false);
		//m_MySQL.EndTransaction(ok);

		r = 1;
		return true;
	}

	void ServerData::OnUpdate()
	{
	}

	void ServerData::OnUpdatePerSecond()
	{
	}

	void ServerData::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
		if (cmd.compare("getuser") == 0)
		{
			if (param.size() > 0)
			{
				cout << "getuser " << param[0] << endl;
			}
			else
			{
				cout << "getuser need param" << endl;
			}
		}
	}

	bool ServerData::OnReleaseStep(int step, float &r)
	{
		m_MySQL.Release();

		r = 1;
		return true;
	}

	void ServerData::OnConnect(NetDataItem *item)
	{
		cout << "ServerData::OnConnect uid:" << item->uid << " key:" << item->key << endl;
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
		if (item->key.first == RemoteType::RT_UNKNOW)
		{
			pNetMgr->Update(item->uid, RemoteKey(RemoteType::RT_CLIENT, 1));
		}
		
		pNetMgr->Send(RemoteKey(RemoteType::RT_CLIENT, 1), 1050, recvdata, index);
		//pNetMgr->Close(RemoteKey(RemoteType::RT_CLIENT, 1));
	}

	void ServerData::F2(NetDataItem *item)
	{
		NetMessageIntString req;
		req.Unpack(item->data, 0);
		cout << "result:" << req.Value1 << " text:" << req.Value2 << endl;
	}
}