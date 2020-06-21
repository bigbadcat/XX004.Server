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
#include "LoginDefine.h"
#include "../../ServerGame.h"
#include <Frame/MainBase.h>
#include <Frame/NetManagerBase.h>
#include <Frame/StorageManager.h>
#include <Frame/EventManager.h>
#include <MySQL/MySQLWrap.h>
#include <Protocol/NetMsgLogin.h>
#include <Util/StringUtil.h>
#include <Util/TimeUtil.h>
#include <Config/BasicModuleConfig.h>

namespace XX004
{
	const int LoginModule::ROLE_ID_STAMP_BIT = 1000000;

	const int LoginModule::OUTLINE_DELAY_NORMAL = 5;

	const int LoginModule::OUTLINE_DELAY_EXCEPTION = 60;

	LoginModule::LoginModule()
	{
	}

	LoginModule::~LoginModule()
	{
		SAFE_DELETE_MAP(m_Users);
	}

	void LoginModule::RegisterNetMessage(NetManagerBase *pMgr)
	{
		NET_REGISTER(pMgr, MsgID::CS_LOGIN, OnLoginRequest);
		NET_REGISTER(pMgr, MsgID::CS_CREATE_ROLE, OnCreateRoleRequest);
		NET_REGISTER(pMgr, MsgID::CS_ENTER_GAME, OnEnterGameRequest);
		NET_REGISTER(pMgr, MsgID::CS_QUIT_GAME, OnQuitGameRequest);
		NET_REGISTER(pMgr, MsgID::DS_ROLE_LIST, OnRoleListResponse);
		NET_REGISTER(pMgr, MsgID::DS_ROLE_ADD, OnRoleAddResponse);
	}

	void LoginModule::RegisterStorageMessage(StorageManager *pMgr)
	{
		DB_REGISTER(pMgr, MsgID::SD_ROLE_LIST, OnRoleListRequest);
		DB_REGISTER(pMgr, MsgID::SD_ROLE_ADD, OnRoleAddRequest);
	}

	void LoginModule::AddConfig(vector<ModuleConfig*> &cfgs)
	{
		cfgs.push_back(ModuleConfig::GetInstance<BasicModuleConfig>());
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
			UserInfo::NextRoleStamp = ret->GetInt("stamp") + 1;
		}
		printf_s("NextRoleStamp:%I64d\n", UserInfo::NextRoleStamp);
	}

	void LoginModule::Release()
	{
		ModuleBase::Release();
	}

	void LoginModule::OnUpdatePerSecond()
	{
		Int64 now = TimeUtil::GetCurrentSecond();
		for (UserInfoVector::iterator itr = m_WantQuitUsers.begin(); itr != m_WantQuitUsers.end(); )
		{
			//判断是否已经到了退出时刻
			if ((*itr)->GetQuitTimeStamp() <= now)
			{
				//移除
				UserInfo *info = *itr;
				m_Users.erase(info->GetName());
				itr = m_WantQuitUsers.erase(itr);
				::printf("UserQuit name:%s rid:%I64d\n", info->GetName().c_str(), info->GetCurRoleID());

				//通知下线，销毁
				EventParam *ep = EventParam::Get(LoginEvent::EID_USER_QUIT);
				ep->l1 = info->GetCurRoleID();
				EventManager::GetInstance()->TriggerEvent(ep);
				SAFE_DELETE(info);
			}
			else
			{
				++itr;
			}
		}
	}

	Int64 LoginModule::GetUID(const string& username)
	{
		UserInfo *info = GetUser(username);
		return info == NULL ? 0 : info->GetUID();
	}

	UserInfo* LoginModule::GetUser(const string& username)
	{
		UserInfo *info = NULL;
		UserInfoMap::iterator itr = m_Users.find(username);
		if (itr != m_Users.end())
		{
			info = itr->second;
		}
		return info;
	}

	UserInfo* LoginModule::GetUser(Int64 uid)
	{
		UIDToUserMap::iterator itr = m_UIDToUserName.find(uid);
		if (itr == m_UIDToUserName.end())
		{
			return NULL;
		}
		return GetUser(itr->second);
	}

	void LoginModule::KickUser(const string& username)
	{
		//断掉连接，退出角色
		UserInfo *info = GetUser(username);
		if (info != NULL)
		{
			MainBase::GetCurMain()->GetNetManager()->Close(info->GetUID());
			OnUserOutline(username, 0, true);
		}		
	}

	void LoginModule::OnConnect(NetDataItem *item)
	{
	}

	void LoginModule::OnDisconnect(NetDataItem *item)
	{
		UserInfo *info = GetUser(item->uid);
		if (info != NULL)
		{
			OnUserOutline(info->GetName(), 1, true);
		}
	}

	void LoginModule::AddUserInfo(const string& username, UInt64 uid)
	{
		UserInfoMap::iterator itr = m_Users.find(username);
		if (itr != m_Users.end())
		{
			//将原来的踢掉线
			UserInfo *ori_info = itr->second;
			MainBase::GetCurMain()->GetNetManager()->Close(ori_info->GetUID());
			if (!ori_info->IsOnline())
			{
				UserInfoVector::iterator ritr = remove(m_WantQuitUsers.begin(), m_WantQuitUsers.end(), ori_info);
				m_WantQuitUsers.erase(ritr, m_WantQuitUsers.end());
			}
			if (ori_info->GetCurRoleID() != 0)
			{
				//角色也踢掉线
				EventParam *ep = EventParam::Get(LoginEvent::EID_USER_QUIT);
				ep->l1 = ori_info->GetCurRoleID();
				EventManager::GetInstance()->TriggerEvent(ep);
			}
			SAFE_DELETE(ori_info);
			m_Users.erase(username);
		}

		UserInfo *info = new UserInfo(username);
		info->SetUID(uid);
		info->SetOnline();
		m_Users.insert(UserInfoMap::value_type(username, info));
		m_UIDToUserName[uid] = username;
	}

	void LoginModule::OnUserOutline(const string& username, int type, bool quit)
	{
		UserInfoMap::iterator itr = m_Users.find(username);
		if (itr == m_Users.end() || !itr->second->IsOnline())
		{
			return;
		}
		printf_s("OnUserOutline user:%s type:%d\n", username.c_str(), type);

		UserInfo *info = itr->second;
		if (info->GetCurRoleID() != 0)
		{
			//通知掉线
			EventParam *ep = EventParam::Get(LoginEvent::EID_USER_OUTLINE);
			ep->l1 = info->GetCurRoleID();
			EventManager::GetInstance()->TriggerEvent(ep);

			//被动掉线停留时间比较长，且可以重连
			int gap = type == 0 ? OUTLINE_DELAY_NORMAL : OUTLINE_DELAY_EXCEPTION;
			info->SetOutline(gap, type == 1);
			if (quit)
			{
				m_WantQuitUsers.push_back(info);
			}			
		}
		else
		{
			//还没选角进游戏，直接删除
			itr = m_Users.erase(itr);
			SAFE_DELETE(info);
		}
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
			MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_LOGIN, &res);
			return;
		}
		AddUserInfo(user_time[0], item->uid);

		//数据请求
		SDRoleListRequest req2;
		req2.UserName = user_time[0];
		RequestStorage(0, MsgID::SD_ROLE_LIST, &req2);
	}

	void LoginModule::OnCreateRoleRequest(NetDataItem *item)
	{
		CSCreateRoleRequest req;
		req.Unpack(item->data, 0);
		printf_s("LoginModule::OnCreateRoleRequest uid:%I64d prof:%d\n", item->uid, req.Prof);

		//判断玩家是否登陆(客户端没叛变是不会有这个问题)
		SCCreateRoleResponse res;
		UserInfo *info = GetUser(item->uid);
		if (info == NULL)
		{
			res.Result = 4;
			MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_CREATE_ROLE, &res);
			return;
		}

		//判断名称是否合法
		if (!UserInfo::CheckRoleName(req.RoleName))
		{
			res.Result = 1;
			MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_CREATE_ROLE, &res);
			return;
		}

		//判断职业是否存在
		ProfConfig *cfg = ModuleConfig::GetInstance<BasicModuleConfig>()->GetProf(req.Prof);
		if (cfg == NULL)
		{
			res.Result = 2;
			MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_CREATE_ROLE, &res);
			return;
		}

		//判断玩家角色是否满了
		if ((int)info->GetRoleInfos().size() >= UserInfo::MAX_ROLE_NUMBER)
		{
			res.Result = 3;
			MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_CREATE_ROLE, &res);
			return;
		}

		//通知数据库保存
		SDRoleAddRequest req2;
		req2.UserName = info->GetName();
		req2.RoleName = req.RoleName;
		req2.Prof = req.Prof;
		RequestStorage(0, MsgID::SD_ROLE_ADD, &req2);
	}

	void LoginModule::OnEnterGameRequest(NetDataItem *item)
	{
		CSEnterGameRequest req;
		req.Unpack(item->data, 0);
		printf_s("LoginModule::OnEnterGameRequest uid:%I64d role:%I64d\n", item->uid, req.RoleID);

		//判断玩家是否登陆(客户端没叛变是不会有这个问题)
		SCEnterGameResponse res;
		UserInfo *info = GetUser(item->uid);
		if (info == NULL)
		{
			res.Result = 3;
			MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_ENTER_GAME, &res);
			return;
		}

		//判断角色是否存在
		RoleInfo *roleinfo = info->GetRoleInfo(req.RoleID);
		if (info == NULL)
		{
			res.Result = 1;
			MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_ENTER_GAME, &res);
			return;
		}

		//判断角色是否被冻结
		if (roleinfo->FreeTime > TimeUtil::GetCurrentSecond())
		{
			res.Result = 2;
			res.FreeTime = roleinfo->FreeTime;
			MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_ENTER_GAME, &res);
			return;
		}

		//将连接与角色id关联，回复登陆成功
		info->SetCurRoleID(roleinfo->ID);
		MainBase::GetCurMain()->GetNetManager()->Update(item->uid, RemoteKey(RemoteType::RT_CLIENT, roleinfo->ID));
		SendNet(roleinfo->ID, MsgID::SC_ENTER_GAME, &res);

		//通知玩家上线，发送初始化数据
		EventParam *ep = EventParam::Get(LoginEvent::EID_USER_ONLINE);
		ep->s1 = info->GetName();
		ep->l1 = info->GetCurRoleID();
		EventManager::GetInstance()->TriggerEvent(ep);
	}

	void LoginModule::OnQuitGameRequest(NetDataItem *item)
	{
		CSQuitGameRequest req;
		req.Unpack(item->data, 0);
		printf_s("LoginModule::OnQuitGameRequest uid:%I64d\n", item->uid);

		UserInfo *info = GetUser(item->uid);
		if (info == NULL)
		{
			//已经断线了 不用再理
			return;
		}

		SCQuitGameResponse res;
		res.Type = req.Type;
		MainBase::GetCurMain()->GetNetManager()->Send(item->uid, MsgID::SC_QUIT_GAME, &res);

		//退出
		OnUserOutline(info->GetName(), 0, req.Type != 2);
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
		int sid = GetServer()->GetServerID();
		sprintf_s(sql, "call sp_select_user_role('%s',%d);", req.UserName.c_str(), sid);
		auto ret = GetMySQL()->Query(sql);
		while (ret->GetRecord())
		{
			LoginRoleInfo role;
			role.ID = ret->GetInt64("id");
			role.Prof = ret->GetInt("prof");
			role.CreateTime = ret->GetInt64("create_time");
			role.FreeTime = ret->GetInt64("free_time");
			role.Name = ret->GetString("name");
			role.Level = ret->GetInt("level");
			res.RoleList.push_back(role);
		}
		res.RoleCount = (Int32)res.RoleList.size();
		ResponseStorage(0, MsgID::DS_ROLE_LIST, &res);
	}

	void LoginModule::OnRoleListResponse(NetDataItem *item)
	{
		DSRoleListResponse res;
		res.Unpack(item->data, 0);
		printf_s("LoginModule::OnRoleListResponse user:%s count:%d\n", res.UserName.c_str(), res.RoleCount);

		UserInfo *info = GetUser(res.UserName);
		if (info == NULL)
		{
			//已经断线了 不用再理
			return;
		}

		//保存玩家信息
		info->GetRoleInfos().clear();
		for (vector<LoginRoleInfo>::iterator itr = res.RoleList.begin(); itr != res.RoleList.end(); ++itr)
		{
			RoleInfo role;
			role.ID = itr->ID;
			role.Prof = itr->Prof;
			role.CreateTime = itr->CreateTime;
			role.FreeTime = itr->FreeTime;
			role.Name = itr->Name;
			role.Level = itr->Level;
			info->AddRoleInfo(role);
		}

		//返回给客户端
		SCLoginResponse res2;
		res2.RoleCount = res.RoleCount;
		res2.RoleList.assign(res.RoleList.begin(), res.RoleList.end());
		MainBase::GetCurMain()->GetNetManager()->Send(info->GetUID(), MsgID::SC_LOGIN, &res2);
	}

	void LoginModule::OnRoleAddRequest(NetDataItem *item)
	{
		SDRoleAddRequest req;
		req.Unpack(item->data, 0);
		printf_s("LoginModule::OnRoleAddRequest user:%s\n", req.UserName.c_str());

		DSRoleAddResponse res;
		res.UserName = req.UserName;

		//名称重复判断
		MySQLWrap *mysql = GetMySQL();
		char sql[128];
		sprintf_s(sql, "call sp_select_role_by_name('%s');", req.RoleName.c_str());
		auto ret = mysql->Query(sql);
		if (ret->GetRecord())
		{
			Int64 roleid = ret->GetInt64("id");
			if (roleid != 0)
			{
				res.Result = 6;
				ResponseStorage(0, MsgID::DS_ROLE_ADD, &res);
				return;
			}
		}

		//数据库级的角色数量判断
		int sid = GetServer()->GetServerID();
		sprintf_s(sql, "call sp_select_user_role_count('%s',%d);", req.UserName.c_str(), sid);
		ret = mysql->Query(sql);
		if (ret->GetRecord())
		{
			int count = ret->GetInt("count");
			if (count >= UserInfo::MAX_ROLE_NUMBER)
			{
				res.Result = 3;
				ResponseStorage(0, MsgID::DS_ROLE_ADD, &res);
				return;
			}
		}

		//判断服务器创建状态或者是否正在创建
		Int64 roleid = UserInfo::GetNewID();		//此函数会导致Stamp增加，所以放在最后，以免创建失败也会导致Stamp增加
		if (roleid == 0)
		{
			res.Result = 5;
			ResponseStorage(0, MsgID::DS_ROLE_ADD, &res);
			return;
		}

		//创建角色
		LoginRoleInfo role;
		role.ID = roleid;
		role.Prof = req.Prof;
		role.CreateTime = (Int64)TimeUtil::GetCurrentSecond();
		role.Name = req.RoleName;
		role.Level = 1;
		res.Role = role;

		//保存角色
		int stamp = (int)(roleid % UserInfo::ROLE_ID_STAMP_BIT);
		sprintf_s(sql, "call sp_insert_update_role(%I64d,'%s','%s',%d,%d,%d,%d,%d,%d,%I64d);",
			role.ID, role.Name.c_str(), req.UserName.c_str(), sid, stamp, role.Prof, role.Level, 0, 0, role.CreateTime);
		mysql->StartTransaction();
		mysql->Execute(sql);
		mysql->EndTransaction(true);

		//返回
		ResponseStorage(0, MsgID::DS_ROLE_ADD, &res);
	}

	void LoginModule::OnRoleAddResponse(NetDataItem *item)
	{
		DSRoleAddResponse res;
		res.Unpack(item->data, 0);
		printf_s("LoginModule::OnRoleAddResponse user:%s result:%d\n", res.UserName.c_str(), res.Result);

		UserInfo *info = GetUser(res.UserName);
		if (info == NULL)
		{
			//已经断线了 不用再理
			return;
		}

		SCCreateRoleResponse res2;
		res2.Result = res.Result;
		if (res2.Result == 0)
		{
			//记录创建的角色
			RoleInfo role;
			role.ID = res.Role.ID;
			role.Prof = res.Role.Prof;
			role.CreateTime = res.Role.CreateTime;
			role.Name = res.Role.Name;
			role.Level = res.Role.Level;
			info->AddRoleInfo(role);
			res2.Role = res.Role;
		}
		MainBase::GetCurMain()->GetNetManager()->Send(info->GetUID(), MsgID::SC_CREATE_ROLE, &res2);
	}
}