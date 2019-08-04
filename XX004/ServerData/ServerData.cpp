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
#include "MainBase.h"
#include <Protocol/NetProtocol.h>
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
		NET_REGISTER(pMgr, NetMsgID::LD_USER_INFO_REQ, OnUserInfoRequest);
		NET_REGISTER(pMgr, NetMsgID::LD_USER_SAVE_REQ, OnUserSaveRequest);
		NET_REGISTER(pMgr, NetMsgID::LD_ROLE_STAMP_REQ, OnRoleStampRequest);
		NET_REGISTER(pMgr, NetMsgID::LD_ROLE_ADD_REQ, OnRoleAddRequest);
		NET_REGISTER(pMgr, NetMsgID::WD_ROLE_BASE_INFO_REQ, OnRoleBaseInfoRequest);
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

	void ServerData::OnUserInfoRequest(NetDataItem *item)
	{
		LDUserInfoRequest req;
		req.Unpack(item->data, 0);

		//查询玩家数据
		DLUserInfoResponse res;
		res.UserName = req.UserName;
		char sql[64];
		sprintf_s(sql, "call sp_select_user('%s');", req.UserName.c_str());
		auto ret = m_MySQL.Query(sql);
		if (ret->GetRecord())
		{
			res.CreateTime = ret->GetInt64("create_time");
			res.FreeTime = ret->GetInt64("free_time");
			ret->Clear();		//再次查询时，ret将处于非法状态，先Clear保证状态正确

			//查询角色
			sprintf_s(sql, "call sp_select_user_role('%s',%d);", req.UserName.c_str(), GetServerKey());
			ret = m_MySQL.Query(sql);
			while (ret->GetRecord())
			{
				LoginRoleInfo role;
				role.ID = ret->GetInt64("id");
				role.Prof = ret->GetInt("prof");
				role.CreateTime = ret->GetInt64("create_time");
				role.Name = ret->GetString("name");
				role.Level = ret->GetInt("level");
				res.RoleList.push_back(role);
			}
			res.RoleCount = (Int32)res.RoleList.size();
		}
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_LOGIN, 1), NetMsgID::DL_USER_INFO_RES, &res);
	}

	void ServerData::OnUserSaveRequest(NetDataItem *item)
	{
		LDUserSaveRequest req;
		req.Unpack(item->data, 0);

		char sql[128];
		sprintf_s(sql, "call sp_insert_update_user('%s',%I64d,%I64d);", req.UserName.c_str(), req.CreateTime, req.FreeTime);
		m_MySQL.Execute(sql);
	}

	void ServerData::OnRoleStampRequest(NetDataItem *item)
	{
		LDRoleStampRequest req;
		req.Unpack(item->data, 0);

		DLRoleStampResponse res;
		int key = req.Group * 10000 + req.ID;
		char sql[64];
		sprintf_s(sql, "call sp_select_role_stamp(%d);", key);
		auto ret = m_MySQL.Query(sql);
		if (ret->GetRecord())
		{
			res.Stamp = ret->GetInt("stamp");
		}
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_LOGIN, 0), NetMsgID::DL_ROLE_STAMP_RES, &res);
	}

	void ServerData::OnRoleAddRequest(NetDataItem *item)
	{
		LDRoleAddRequest req;
		req.Unpack(item->data, 0);

		DLRoleAddResponse res;
		res.UserName = req.UserName;
		res.Role = req.Role;

		char sql[128];
		sprintf_s(sql, "call sp_select_role_by_name('%s');", req.Role.Name.c_str());
		auto ret = m_MySQL.Query(sql);
		if (ret->GetRecord())
		{
			Int64 roleid = ret->GetInt64("id");
			if (roleid != 0)
			{
				res.Result = 1;
			}
		}
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_LOGIN, 0), NetMsgID::DL_ROLE_ADD_RES, &res);

		//可以创建，保存角色
		if (res.Result == 0)
		{
			LoginRoleInfo &role = req.Role;
			int sid = GetServerKey();
			sprintf_s(sql, "call sp_insert_update_role(%I64d,'%s','%s',%d,%d,%d,%d,%d,%d,%I64d);", 
				role.ID, role.Name.c_str(), req.UserName.c_str(), sid, req.Stamp, role.Prof, role.Level, 0, 0, role.CreateTime);
			m_MySQL.Execute(sql);
		}
	}

	void ServerData::OnRoleBaseInfoRequest(NetDataItem *item)
	{
		WDRoleBaseInfoRequest req;
		req.Unpack(item->data, 0);

		DWRoleBaseInfoResponse res;
		res.UserName = req.UserName;
		res.ID = req.ID;

		char sql[128];
		sprintf_s(sql, "call sp_select_role(%I64d);", req.ID);
		auto ret = m_MySQL.Query(sql);
		if (ret->GetRecord())
		{
			res.Prof = ret->GetInt("prof");
			res.CreateTime = ret->GetInt64("create_time");
			res.Name = ret->GetString("name");
			res.Level = ret->GetInt("level");
			res.Exp = ret->GetInt64("exp");
			res.Map = ret->GetInt("map");
			res.PositionX = ret->GetInt("pos_x");
			res.PositionY = ret->GetInt("pos_y");
			res.Direction = ret->GetInt("pos_dir");
		}
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_WORLD, 0), NetMsgID::DW_ROLE_BASE_INFO_REQ, &res);
	}
}