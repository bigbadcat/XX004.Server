/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: LoginModule.cpp
* Summary: 登陆模块。
*
* Author: XuXiang
* Date: 2019-10-03 11:27
*******************************************************/

#include "LoginModule.h"
#include "../ServerGame.h"
#include <MainBase.h>
#include <Protocol/NetProtocol.h>
#include <Util/StringUtil.h>
#include <Util/TimeUtil.h>

namespace XX004
{
	LoginModule::LoginModule() : m_NextRoleStamp(0)
	{
	}

	LoginModule::~LoginModule()
	{
	}

	void LoginModule::RegisterNetMessage(NetManagerBase *pMgr)
	{
		NET_REGISTER(pMgr, NetMsgID::CS_LOGIN_REQ, OnLoginRequest);
		NET_REGISTER(pMgr, NetMsgID::DS_ROLE_LIST_RES, OnRoleListResponse);
	}

	void LoginModule::RegisterStorageMessage(StorageManager *pMgr)
	{
		DB_REGISTER(pMgr, NetMsgID::SD_ROLE_LIST_REQ, OnRoleListRequest);
	}

	void LoginModule::Init()
	{
		ModuleBase::Init();

		//读取角色创建序号
		char sql[64];
		sprintf_s(sql, "call sp_select_role_stamp(%d);", GetServer()->GetServerID());
		auto ret = GetMySQL()->Query(sql);
		if (ret->GetRecord())
		{
			m_NextRoleStamp = ret->GetInt("stamp") + 1;
		}
		printf_s("NextRoleStamp:%d\n", m_NextRoleStamp);
	}

	void LoginModule::Release()
	{
		ModuleBase::Release();
	}

	void LoginModule::OnLoginRequest(NetDataItem *item)
	{
		CSLoginRequest req;
		req.Unpack(item->data, 0);
		printf_s("LoginModule::OnLoginRequest token:%s\n", req.Token.c_str());

		//token验证
		vector<string> user_time;
		StringUtil::Split(req.Token, "&", user_time);
		bool ok = user_time.size() == 2;
		if (ok)
		{
			UInt64 t = (UInt64)::_atoi64(user_time[1].c_str());
			UInt64 now = TimeUtil::GetCurrentSecond();
			ok = t < now && (t + 30) >= now;
		}
		if (!ok)
		{
			SCLoginResponse res;
			res.Result = 1;
			SendNet(0, NetMsgID::SC_LOGIN_RES, &res);
			return;
		}

		//数据请求
		SDRoleListRequest req2;
		req2.UserName = user_time[0];
		RequestStorage(0, NetMsgID::SD_ROLE_LIST_REQ, &req2);
	}

	void LoginModule::OnRoleListRequest(NetDataItem *item)
	{
		SDRoleListRequest req;
		req.Unpack(item->data, 0);
		printf_s("LoginModule::OnRoleListRequest user:%s\n", req.UserName.c_str());

		//查询玩家角色数据
		DSRoleListResponse res;
		res.UserName = req.UserName;
		char sql[64];
		int sid = dynamic_cast<ServerGame*>(MainBase::GetCurMain()->GetServe())->GetServerID();
		sprintf_s(sql, "call sp_select_user_role('%s',%d);", req.UserName.c_str(), sid);
		auto ret = GetMySQL()->Query(sql);
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
		ResponseStorage(0, NetMsgID::DS_ROLE_LIST_RES, &res);
	}

	void LoginModule::OnRoleListResponse(NetDataItem *item)
	{
		DSRoleListResponse res;
		res.Unpack(item->data, 0);
		printf_s("LoginModule::OnRoleListResponse user:%s count:%d\n", res.UserName.c_str(), res.RoleCount);

		SCLoginResponse res2;
		res2.RoleCount = res.RoleCount;
		res2.RoleList.assign(res.RoleList.begin(), res.RoleList.end());
		SendNet(0, NetMsgID::SC_LOGIN_RES, &res2);
	}
}