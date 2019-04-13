/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerLogin.cpp
* Summary: 网关服务模块。
*
* Author: XuXiang
* Date: 2018-12-18 19:56
*******************************************************/

#include "ServerLogin.h"
#include <NetManagerBase.h>
#include <Protocol/NetProtocol.h>
#include <MainBase.h>
#include <Config/LoginModuleConfig.h>
#include <Util/TimeUtil.h>
#include <Util/StringUtil.h>
#include <regex>
using namespace XX004::Net;

namespace XX004
{
	ServerLogin::ServerLogin()
	{
	}

	ServerLogin::~ServerLogin()
	{
		SAFE_DELETE_MAP(m_UserInfos);
	}

	//检测名称字符串
	bool CheckNameChar(char c)
	{
		if (c >= '0' && c <= '9')
		{
			return true;
		}
		if (c >= 'A' && c <= 'Z')
		{
			return true;
		}
		if (c >= 'a' && c <= 'z')
		{
			return true;
		}
		return c == '_';
	}

	const int ServerLogin::MAX_ROLE_NUMBER = 5;

	bool ServerLogin::CheckName(const std::string& name)
	{
		int index = 0;
		while (index < name.length())
		{
			char c = name[index];
			if (c & 0x80)
			{
				index += 3;		//汉字UTF-8占三个字节(遇到占两个字节的字符编码会不正确)
			}
			else
			{
				//非汉字只能是数字、字母和下划线
				if (CheckNameChar(c))
				{
					++index;
				}
				else
				{
					return false;
				}
			}
		}

		return true;
	}

	void ServerLogin::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		NET_REGISTER(pMgr, NetMsgID::GL_LOGIN_REQ, OnLoginRequest);
		NET_REGISTER(pMgr, NetMsgID::DL_USER_INFO_RES, OnUserInfoResponse);
		NET_REGISTER(pMgr, NetMsgID::CL_CREATE_ROLE_REQ, OnCreateRoleRequest);
		NET_REGISTER(pMgr, NetMsgID::DL_ROLE_STAMP_RES, OnRoleStampResponse);
		NET_REGISTER(pMgr, NetMsgID::DL_ROLE_ADD_RES, OnRoleAddResponse);
		NET_REGISTER(pMgr, NetMsgID::GL_ENTER_GAME_REQ, OnEnterGameRequest);
	}

	void ServerLogin::OnAddConfig(vector<ModuleConfig*> &cfgs)
	{
		cfgs.push_back(ModuleConfig::GetInstance<LoginModuleConfig>());
	}

	bool ServerLogin::OnInitStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerLogin::OnUpdate()
	{
	}

	void ServerLogin::OnUpdatePerSecond()
	{
	}

	void ServerLogin::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
	}

	bool ServerLogin::OnReleaseStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerLogin::OnConnect(NetDataItem *item)
	{
		if (item->key.first == RemoteType::RT_DATA)
		{
			LDRoleStampRequest req;
			req.Group = this->GetServerGroup();
			req.ID = this->GetServerID();
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_ROLE_STAMP_REQ, &req);
		}
	}

	void ServerLogin::OnDisconnect(NetDataItem *item)
	{
	}

	void ServerLogin::OnLoginRequest(NetDataItem *item)
	{
		GLLoginRequest req;
		req.Unpack(item->data, 0);
		
		//若是非法用户名直接返回
		bool ok = true;
		if (req.UserName.length() < 4 || req.UserName.length() > 16)
		{
			ok = false;
		}
		if (ok)
		{
			//正则匹配格式
			regex reg("^[a-zA-Z0-9_]*$");
			ok = regex_match(req.UserName, reg);
		}

		//用户名合法则请求用户信息，否则返回用户名非法
		if (ok)
		{
			UserInfoMap::iterator itr = m_UserInfos.find(req.UserName);
			if (itr == m_UserInfos.end())
			{
				//数据库查询
				LDUserInfoRequest req2;
				req2.UserName = req.UserName;
				MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_INFO_REQ, &req);
			}
			else
			{
				//将结果回复给玩家
				UserInfo* info = itr->second;
				Int64 now = (Int64)TimeUtil::GetCurrentSecond();
				LGLoginResponse res2;
				res2.Result = info->GetFreeTime() <= now ? 0 : 2;
				res2.UserName = info->GetName();
				res2.FreeTime = info->GetFreeTime();
				const vector<LoginRoleInfo>& roles = info->GetRoleInfos();
				res2.RoleCount = (Int32)roles.size();
				res2.RoleList.assign(roles.begin(), roles.end());
				MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_LOGIN_RES, &res2);
			}
		}
		else
		{
			LGLoginResponse res2;
			res2.Result = 1;
			res2.UserName = req.UserName;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_LOGIN_RES, &res2);
		}		
	}

	void ServerLogin::OnUserInfoResponse(NetDataItem *item)
	{
		DLUserInfoResponse res;
		res.Unpack(item->data, 0);

		//账号不存在则自动创建
		if (res.CreateTime == 0)
		{
			//提交保存
			LDUserSaveRequest req;
			req.UserName = res.UserName;
			req.CreateTime = (Int64)TimeUtil::GetCurrentSecond();
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_SAVE_REQ, &req);

			//重新请求，走正确时序流程
			LDUserInfoRequest req2;
			req2.UserName = res.UserName;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_USER_INFO_REQ, &req2);
		}
		else
		{
			//保存玩家信息
			UserInfoMap::iterator itr = m_UserInfos.find(res.UserName);
			UserInfo *info = itr == m_UserInfos.end() ? NULL : itr->second;
			if (info == NULL)
			{
				//添加玩家
				info = new UserInfo(res.UserName);
				m_UserInfos.insert(make_pair(res.UserName, info));
			}
			info->SetFreeTime(res.FreeTime);
			info->GetRoleInfos().assign(res.RoleList.begin(), res.RoleList.end());

			//将结果回复给玩家
			Int64 now = (Int64)TimeUtil::GetCurrentSecond();
			LGLoginResponse res2;
			res2.Result = res.FreeTime <= now ? 0 : 2;
			res2.UserName = res.UserName;
			res2.FreeTime = res.FreeTime;
			res2.RoleCount = res.RoleCount;
			res2.RoleList.assign(res.RoleList.begin(), res.RoleList.end());
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_LOGIN_RES, &res2);
		}
	}

	void ServerLogin::OnCreateRoleRequest(NetDataItem *item)
	{
		CLCreateRoleRequest req;
		req.Unpack(item->data, 0);

		LGCreateRoleResponse res;
		res.UserName = req.UserName;

		//判断名称是否合法
		int width = StringUtil::GetStringWidth(req.RoleName);
		if (width < 4 || width > 12 || !CheckName(req.RoleName))
		{
			res.Result = 1;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_CREATE_ROLE_RES, &res);
			return;
		}

		//判断职业是否存在
		ProfConfig *cfg = ModuleConfig::GetInstance<LoginModuleConfig>()->GetProf(req.Prof);
		if (cfg == NULL)
		{
			res.Result = 2;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_CREATE_ROLE_RES, &res);
			return;
		}

		//判断玩家是否登陆(客户端没叛变是不会有这个问题)
		UserInfoMap::iterator itr = m_UserInfos.find(res.UserName);
		UserInfo *info = itr == m_UserInfos.end() ? NULL : itr->second;
		if (info == NULL)
		{
			res.Result = 4;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_CREATE_ROLE_RES, &res);
			return;
		}

		//判断玩家角色是否满了
		if ((int)info->GetRoleInfos().size() >= MAX_ROLE_NUMBER)
		{
			res.Result = 3;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_CREATE_ROLE_RES, &res);
			return;
		}

		//判断服务器创建状态或者是否正在创建
		Int64 roleid = UserInfo::GetNewID();
		if (roleid == 0)
		{
			res.Result = 5;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_CREATE_ROLE_RES, &res);
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

		//通知数据库保存
		LDRoleAddRequest req2;
		req2.UserName = req.UserName;
		req2.Role = role;
		req2.Stamp = roleid % UserInfo::ROLE_ID_STAMP_BIT;
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_DATA, 0), NetMsgID::LD_ROLE_ADD_REQ, &req2);
	}

	void ServerLogin::OnRoleStampResponse(NetDataItem *item)
	{
		DLRoleStampResponse res;
		res.Unpack(item->data, 0);
		UserInfo::NextRoleStamp = res.Stamp + 1;
		cout << "ServerLogin::OnRoleStampResponse stamp:" << res.Stamp << endl;
	}

	void ServerLogin::OnRoleAddResponse(NetDataItem *item)
	{
		DLRoleAddResponse res;
		res.Unpack(item->data, 0);

		LGCreateRoleResponse res2;
		res2.UserName = res.UserName;		
		res2.Result = res.Result == 0 ? 0 : 6;
		if (res2.Result == 0)
		{
			res2.Role = res.Role;

			//记录创建的角色
			UserInfoMap::iterator itr = m_UserInfos.find(res.UserName);
			UserInfo *info = itr == m_UserInfos.end() ? NULL : itr->second;
			if (info != NULL)
			{
				info->AddRoleInfo(res2.Role);
			}
		}
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, 0), NetMsgID::LG_CREATE_ROLE_RES, &res2);
	}

	void ServerLogin::OnEnterGameRequest(NetDataItem *item)
	{
		GLEnterGameRequest req;
		req.Unpack(item->data, 0);

		//判断玩家是否登陆(客户端没叛变是不会有这个问题)
		UserInfoMap::iterator itr = m_UserInfos.find(req.UserName);
		UserInfo *info = itr == m_UserInfos.end() ? NULL : itr->second;
		if (info == NULL)
		{
			LCEnterGameFailed res;
			res.Result = 1;
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, req.RoleID), NetMsgID::LC_ENTER_GAME_FAILED, &res);
			return;
		}

		//判断角色是否存在

		//判断角色是否被冻结

		//可以成功进入游戏
		cout << "EnterGame username:" << req.UserName << " roleid:" << req.RoleID << endl;
	}
}