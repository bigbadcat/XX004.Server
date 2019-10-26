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
#include "PlayerBasicData.h"
#include "PlayerDefine.h"
#include <Frame/NetManagerBase.h>
#include <Frame/StorageManager.h>
#include <Protocol/NetProtocol.h>
#include <MySQL/MySQLWrap.h>
#include <Util/StringUtil.h>
#include <Util/TimeUtil.h>
#include <Config/BasicModuleConfig.h>
#include "../Login/LoginDefine.h"
#include "../Login/UserInfo.h"

namespace XX004
{

	PlayerModule::PlayerModule()
	{
	}

	PlayerModule::~PlayerModule()
	{
		SAFE_DELETE_MAP(m_Players);
	}

	void PlayerModule::RegisterNetMessage(NetManagerBase *pMgr)
	{
		NET_REGISTER(pMgr, NetMsgID::CS_RENAME_REQ, OnRenameRequest);
		NET_REGISTER(pMgr, NetMsgID::DS_BASIC_INFO_RES, OnDBBasicInfoResponse);
		NET_REGISTER(pMgr, NetMsgID::DS_RENAME_RES, OnDBRenameResponse);

		EventManager *emgr = EventManager::GetInstance();
		emgr->RegisterCallBack(LoginEvent::EID_USER_ONLINE, OnEventUserOnline, this);
		emgr->RegisterCallBack(LoginEvent::EID_USER_OUTLINE, OnEventUserOutline, this);
	}

	void PlayerModule::RegisterStorageMessage(StorageManager *pMgr)
	{
		DB_REGISTER(pMgr, NetMsgID::SD_BASIC_INFO_REQ, OnDBBasicInfoRequest);
		DB_REGISTER(pMgr, NetMsgID::SD_BASIC_SAVE_REQ, OnDBBasicSaveRequest);
		DB_REGISTER(pMgr, NetMsgID::SD_RENAME_REQ, OnDBRenameRequest);
	}

	void PlayerModule::AddConfig(vector<ModuleConfig*> &cfgs)
	{
		cfgs.push_back(ModuleConfig::GetInstance<BasicModuleConfig>());
	}

	void PlayerModule::Init()
	{
		ModuleBase::Init();
	}

	void PlayerModule::Release()
	{
		ModuleBase::Release();
	}

	PlayerBasicData* PlayerModule::GetPlayer(Int64 rid)
	{
		PlayerMap::iterator itr = m_Players.find(rid);
		return itr == m_Players.end() ? NULL : itr->second;
	}

	void PlayerModule::OnRenameRequest(NetDataItem *item)
	{
		CSRenameRequest req;
		req.Unpack(item->data, 0);
		::printf_s("PlayerModule::OnRenameRequest roleid:%I64d name:%s\n", item->key.second, req.Name.c_str());

		//名称合法性
		if (!UserInfo::CheckRoleName(req.Name))
		{
			SCRenameResponse res;
			res.Result = 1;
			SendNet(item->key.second, NetMsgID::SC_RENAME_RES, &res);
		}

		//判断改名消耗(依赖背包模块)

		//DB请求
		SDRenameRequest req2;
		req2.Name = req.Name;
		RequestStorage(item->key.second, NetMsgID::SD_RENAME_REQ, &req2);
	}

	void PlayerModule::OnDBBasicInfoRequest(NetDataItem *item)
	{
		//读取信息
		Int64 rid = item->key.second;
		DSBasicInfoResponse res;
		MySQLWrap *mysql = GetMySQL();
		char sql[128];
		sprintf_s(sql, "call sp_select_role(%I64d);", rid);
		auto ret = mysql->Query(sql);
		if (ret->GetRecord())
		{
			res.Prof = ret->GetInt("prof");
			res.Name = ret->GetString("name");
			res.Level = ret->GetInt("level");
			res.Exp = ret->GetInt64("exp");
			res.Map = ret->GetInt("map");
			res.PositionX = ret->GetInt("pos_x");
			res.PositionY = ret->GetInt("pos_y");
			res.Direction = ret->GetInt("pos_dir");
		}
		ret->Clear();
		ResponseStorage(rid, NetMsgID::DS_BASIC_INFO_RES, &res);
	}

	void PlayerModule::OnDBBasicInfoResponse(NetDataItem *item)
	{
		PlayerBasicData *player = GetPlayer(item->key.second);
		if (player == NULL)
		{
			return;
		}

		DSBasicInfoResponse res;
		res.Unpack(item->data, 0);
		player->m_Prof = res.Prof;
		player->m_Name = res.Name;
		player->m_Level = res.Level;
		player->m_Exp = res.Exp;
		player->m_PosMap = res.Map;
		player->m_PosX = res.PositionX;
		player->m_PosY = res.PositionY;
		player->m_PosDir = res.Direction;
		player->SetSelfDirty();
		player->SetInitMark(PlayerBasicData::MODEL_ID, true);
	}
	void PlayerModule::OnDBBasicSaveRequest(NetDataItem *item)
	{

	}

	void PlayerModule::OnDBRenameRequest(NetDataItem *item)
	{
		SDRenameRequest req;
		req.Unpack(item->data, 0);

		DSRenameResponse res;
		res.Name = req.Name;

		//名称重复判断
		MySQLWrap *mysql = GetMySQL();
		char sql[128];
		sprintf_s(sql, "call sp_select_role_by_name('%s');", req.Name.c_str());
		auto ret = mysql->Query(sql);
		if (ret->GetRecord())
		{
			Int64 roleid = ret->GetInt64("id");
			if (roleid != 0)
			{
				res.Result = 2;
				ResponseStorage(0, NetMsgID::DS_RENAME_RES, &res);
				return;
			}
		}
		ret->Clear();

		//保存名称
		sprintf_s(sql, "call sp_update_role_name(%I64d,'%s');", item->key.second, req.Name.c_str());
		mysql->Execute(sql);

		//回复
		res.Result = 0;
		ResponseStorage(0, NetMsgID::DS_RENAME_RES, &res);
	}

	void PlayerModule::OnDBRenameResponse(NetDataItem *item)
	{
		PlayerBasicData *player = GetPlayer(item->key.second);
		if (player == NULL)
		{
			return;
		}

		//更新结果
		DSRenameResponse res;
		res.Unpack(item->data, 0);
		if (res.Result == 0)
		{
			player->SetName(res.Name);
		}

		//回复玩家
		SCRenameResponse res2;
		res2.Result = res.Result;
		res2.Name = res.Name;
		SendNet(player->GetID(), NetMsgID::CS_RENAME_REQ, &res2);
	}

	void PlayerModule::OnEventUserOnline(int id, EventParam *ep)
	{
		::printf_s("PlayerModule::OnEventUserOnline user:%s roleid:%I64d\n", ep->s1.c_str(), ep->l1);
		PlayerBasicData *player = GetPlayer(ep->l1);
		if (player != NULL)
		{
			//重新同步数据给客户端即可
			player->SendToClient();

			//通知相关模块同步
			EventParam *ep2 = EventParam::Get(PlayerEvent::EDI_PLAYER_SYNC_CLIENT);
			ep2->p1 = player;
			EventManager::GetInstance()->TriggerEvent(ep2);
			return;
		}

		//创建新玩家
		player = new PlayerBasicData();
		player->SetUser(ep->s1);
		player->SetID(ep->l1);
		player->SetInitMark(PlayerBasicData::MODEL_ID);
		m_Players.insert(make_pair(player->GetID(), player));

		//发送角色创建事件
		EventParam *ep2 = EventParam::Get(PlayerEvent::EID_PLAYER_CREATE);
		ep2->p1 = player;
		EventManager::GetInstance()->TriggerEvent(ep2);

		//向DB请求基本信息
		SDBasicInfoRequest req;
		RequestStorage(player->GetID(), NetMsgID::SD_BASIC_INFO_REQ, &req);
	}

	void PlayerModule::OnEventUserOutline(int id, EventParam *ep)
	{
		::printf_s("PlayerModule::OnEventUserOutline roleid:%I64d\n", ep->l1);
		PlayerMap::iterator itr = m_Players.find(ep->l1);
		if (itr == m_Players.end())
		{
			return;
		}

		//发送角色销毁事件
		EventParam *ep2 = EventParam::Get(PlayerEvent::EID_PLAYER_DESTROY);
		ep2->p1 = itr->second;
		EventManager::GetInstance()->TriggerEvent(ep2);

		//销毁角色
		SAFE_DELETE(itr->second);
		itr = m_Players.erase(itr);
	}
}