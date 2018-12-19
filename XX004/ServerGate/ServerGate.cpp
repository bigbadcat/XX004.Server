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

	void ServerGate::RegisterNetMessage(NetManagerBase *pMgr)
	{
		pMgr->SetOnConnectCallBack([this](NetDataItem *item){this->OnConnect(item); });
		pMgr->SetOnDisconnectCallBack([this](NetDataItem *item){this->OnDisconnect(item); });
		pMgr->RegisterMessageCallBack(NetMsgID::CG_LOGIN_REQ, [this](NetDataItem *item){this->OnLoginRequest(item); });
		pMgr->RegisterMessageCallBack(NetMsgID::LG_LOGIN_RES, [this](NetDataItem *item){this->OnLoginResponse(item); });
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
}