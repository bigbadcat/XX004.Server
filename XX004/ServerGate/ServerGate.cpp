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
#include <assert.h>
#include <Util/TimeUtil.h>
using namespace XX004::Net;

namespace XX004
{
	ServerGate::ServerGate()
	{
	}

	ServerGate::~ServerGate()
	{
	}

	void ServerGate::SetRelation(Int64 uid, const std::string& username)
	{
		//重复设置是不应该出现的
		UIDToUserNameMap::iterator itr_uid = m_UIDToUserName.find(uid);
		if (itr_uid != m_UIDToUserName.end())
		{
			int cp = itr_uid->second.compare(username);
			assert(cp == 0);
			return;
		}

		UserNameToUIDMap::iterator itr_username = m_UserNameToUID.find(username);
		if (itr_username != m_UserNameToUID.end())
		{
			if (itr_username->second == uid)
			{
				//重复设置
				return;				
			}

			//账号再次登陆，把原来的踢掉线
			NetManagerBase *pNetMgr = MainBase::GetCurMain()->GetNetManager();
			pNetMgr->Close(itr_username->second);
			cout << "Login again. username:" << username << endl;

			//移除关联
			m_UIDToUserName.erase(itr_username->second);
			m_UserNameToUID.erase(username);
		}

		m_UIDToUserName[uid] = username;
		m_UserNameToUID[username] = uid;
	}

	void ServerGate::RemoveRelation(Int64 uid)
	{
		UIDToUserNameMap::iterator itr = m_UIDToUserName.find(uid);
		if (itr != m_UIDToUserName.end())
		{
			m_UserNameToUID.erase(itr->second);
			itr = m_UIDToUserName.erase(itr);
		}
	}

	Int64 ServerGate::GetUID(const string& username)
	{
		UserNameToUIDMap::iterator itr = m_UserNameToUID.find(username);
		return itr == m_UserNameToUID.end() ? 0 : itr->second;
	}

	string ServerGate::GetUserName(Int64 uid)
	{
		UIDToUserNameMap::iterator itr = m_UIDToUserName.find(uid);
		return itr == m_UIDToUserName.end() ? string("") : itr->second;
	}

	void ServerGate::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		NET_REGISTER(pMgr, NetMsgID::CG_LOGIN_REQ, OnLoginRequest);
		NET_REGISTER(pMgr, NetMsgID::CG_ENTER_GAME_REQ, OnEnterGameRequest);
		NET_REGISTER(pMgr, NetMsgID::LG_LOGIN_RES, OnLoginResponse); 
		NET_REGISTER(pMgr, NetMsgID::LG_CREATE_ROLE_RES, OnCreateRoleResponse);
		NET_REGISTER(pMgr, NetMsgID::WG_ENTER_GAME_SUCCESS, OnEnterGameSuccessResponse);
	}

	bool ServerGate::OnInitStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerGate::OnUpdate()
	{
	}

	void ServerGate::OnUpdatePerSecond()
	{
	}

	void ServerGate::OnCommand(const std::string& cmd, const std::vector<std::string> &param)
	{
	}

	bool ServerGate::OnReleaseStep(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerGate::OnConnect(NetDataItem *item)
	{
	}

	void ServerGate::OnDisconnect(NetDataItem *item)
	{
		RemoveRelation(item->uid);
	}

	void ServerGate::OnLoginRequest(NetDataItem *item)
	{
		CGLoginRequest req;
		req.Unpack(item->data, 0);
		cout << "ServerGate::OnLoginRequest username:" << req.UserName << endl;
		SetRelation(item->uid, req.UserName);

		//请求转发给登陆服
		GLLoginRequest lreq;
		lreq.UserName = req.UserName;
		MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_LOGIN, 0), NetMsgID::GL_LOGIN_REQ, &lreq);
	}

	void ServerGate::OnEnterGameRequest(NetDataItem *item)
	{
		CGEnterGameRequest req;
		req.Unpack(item->data, 0);

		NetManagerBase *pNet = MainBase::GetCurMain()->GetNetManager();
		pNet->Update(item->uid, RemoteKey(RemoteType::RT_CLIENT, req.RoleID));			//将连接与RoleID关联

		GLEnterGameRequest req2;
		req2.UserName = GetUserName(item->uid);
		req2.RoleID = req.RoleID;
		pNet->Send(RemoteKey(RemoteType::RT_LOGIN, 0), NetMsgID::GL_ENTER_GAME_REQ, &req2);
	}

	void ServerGate::OnLoginResponse(NetDataItem *item)
	{
		LGLoginResponse res;
		res.Unpack(item->data, 0);

		Int64 uid = GetUID(res.UserName);

		//回复转发给客户端
		GCLoginResponse lres;
		lres.Result = res.Result;
		lres.UserName = res.UserName;
		lres.FreeTime = res.FreeTime;
		lres.RoleCount = res.RoleCount;
		lres.RoleList.assign(res.RoleList.begin(), res.RoleList.end());
		MainBase::GetCurMain()->GetNetManager()->Send(uid, NetMsgID::GC_LOGIN_RES, &lres);
	}

	void ServerGate::OnCreateRoleResponse(NetDataItem *item)
	{
		LGCreateRoleResponse res;
		res.Unpack(item->data, 0);

		Int64 uid = GetUID(res.UserName);
		GCCreateRoleResponse res2;
		res2.Result = res.Result;
		res2.Role = res.Role;
		MainBase::GetCurMain()->GetNetManager()->Send(uid, NetMsgID::GC_CREATE_ROLE_RES, &res2);
	}

	void ServerGate::OnEnterGameSuccessResponse(NetDataItem *item)
	{
		WGEnterGameSuccess res;
		res.Unpack(item->data, 0);

		Int64 uid = GetUID(res.UserName);
		GCEnterGameSuccess res2;
		char token[128];
		::sprintf_s(token, sizeof(token), "%s&%I64d&%I64d", res.UserName.c_str(), res.RoleID, TimeUtil::GetCurrentSecond());
		res2.Token = token;
		MainBase::GetCurMain()->GetNetManager()->Send(uid, NetMsgID::GC_ENTER_GAME_SUCCESS, &res2);
	}
}