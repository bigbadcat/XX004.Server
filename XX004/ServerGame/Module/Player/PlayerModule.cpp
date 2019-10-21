/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerModule.cpp
* Summary: 玩家模块。
*
* Author: XuXiang
* Date: 2019-10-21 20:41
*******************************************************/

#include "PlayerModule.h"
#include <NetManagerBase.h>
#include <StorageManager.h>
#include <Protocol/NetProtocol.h>
#include <Util/StringUtil.h>
#include <Util/TimeUtil.h>

namespace XX004
{

	PlayerModule::PlayerModule()
	{
	}

	PlayerModule::~PlayerModule()
	{
	}

	void PlayerModule::RegisterNetMessage(NetManagerBase *pMgr)
	{
		//NET_REGISTER(pMgr, NetMsgID::CS_LOGIN_REQ, OnLoginRequest);
		//NET_REGISTER(pMgr, NetMsgID::DS_ROLE_LIST_RES, OnRoleListResponse);
	}

	void PlayerModule::RegisterStorageMessage(StorageManager *pMgr)
	{
		//DB_REGISTER(pMgr, NetMsgID::SD_ROLE_LIST_REQ, OnRoleListRequest);
	}

	void PlayerModule::AddConfig(vector<ModuleConfig*> &cfgs)
	{
		//cfgs.push_back(ModuleConfig::GetInstance<PlayerModuleConfig>());
	}

	void PlayerModule::Init()
	{
		ModuleBase::Init();
	}

	void PlayerModule::Release()
	{
		ModuleBase::Release();
	}

	//void PlayerModule::OnLoginRequest(NetDataItem *item)
	//{
	//	CSLoginRequest req;
	//	req.Unpack(item->data, 0);
	//	printf_s("PlayerModule::OnLoginRequest token:%s\n", req.Token.c_str());

	//	//token验证
	//	vector<string> user_time;
	//	StringUtil::Split(req.Token, "&", user_time);
	//	bool ok = user_time.size() == 2;
	//	if (ok)
	//	{
	//		UInt64 t = (UInt64)::_atoi64(user_time[1].c_str());
	//		UInt64 now = TimeUtil::GetCurrentSecond();
	//		ok = t < now && (t + 30) >= now;
	//	}
	//	if (!ok)
	//	{
	//		SCLoginResponse res;
	//		res.Result = 1;
	//		MainBase::GetCurMain()->GetNetManager()->Send(item->uid, NetMsgID::SC_LOGIN_RES, &res);
	//		return;
	//	}
	//	AddUserInfo(user_time[0], item->uid);

	//	//数据请求
	//	SDRoleListRequest req2;
	//	req2.UserName = user_time[0];
	//	RequestStorage(0, NetMsgID::SD_ROLE_LIST_REQ, &req2);
	//}

	//void PlayerModule::OnRoleListRequest(NetDataItem *item)
	//{
	//	SDRoleListRequest req;
	//	req.Unpack(item->data, 0);
	//	printf_s("PlayerModule::OnRoleListRequest user:%s\n", req.UserName.c_str());

	//	//查询玩家角色数据
	//	DSRoleListResponse res;
	//	res.UserName = req.UserName;
	//	char sql[64];
	//	int sid = GetServer()->GetServerID();
	//	sprintf_s(sql, "call sp_select_user_role('%s',%d);", req.UserName.c_str(), sid);
	//	auto ret = GetMySQL()->Query(sql);
	//	while (ret->GetRecord())
	//	{
	//		LoginRoleInfo role;
	//		role.ID = ret->GetInt64("id");
	//		role.Prof = ret->GetInt("prof");
	//		role.CreateTime = ret->GetInt64("create_time");
	//		role.FreeTime = ret->GetInt64("free_time");
	//		role.Name = ret->GetString("name");
	//		role.Level = ret->GetInt("level");
	//		res.RoleList.push_back(role);
	//	}
	//	res.RoleCount = (Int32)res.RoleList.size();
	//	ResponseStorage(0, NetMsgID::DS_ROLE_LIST_RES, &res);
	//}

	//void PlayerModule::OnRoleListResponse(NetDataItem *item)
	//{
	//	DSRoleListResponse res;
	//	res.Unpack(item->data, 0);
	//	printf_s("PlayerModule::OnRoleListResponse user:%s count:%d\n", res.UserName.c_str(), res.RoleCount);

	//	UserInfo *info = GetUser(res.UserName);
	//	if (info == NULL)
	//	{
	//		//已经断线了 不用再理
	//		return;
	//	}

	//	//保存玩家信息
	//	info->GetRoleInfos().clear();
	//	for (vector<LoginRoleInfo>::iterator itr = res.RoleList.begin(); itr != res.RoleList.end(); ++itr)
	//	{
	//		RoleInfo role;
	//		role.ID = itr->ID;
	//		role.Prof = itr->Prof;
	//		role.CreateTime = itr->CreateTime;
	//		role.FreeTime = itr->FreeTime;
	//		role.Name = itr->Name;
	//		role.Level = itr->Level;
	//		info->AddRoleInfo(role);
	//	}

	//	//返回给客户端
	//	SCLoginResponse res2;
	//	res2.RoleCount = res.RoleCount;
	//	res2.RoleList.assign(res.RoleList.begin(), res.RoleList.end());
	//	MainBase::GetCurMain()->GetNetManager()->Send(info->GetUID(), NetMsgID::SC_LOGIN_RES, &res2);
	//}
}