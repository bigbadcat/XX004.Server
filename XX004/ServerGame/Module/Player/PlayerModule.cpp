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
#include <Protocol/NetMsgPlayerBasic.h>
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
		NET_REGISTER(pMgr, MsgID::CS_RENAME, OnRenameRequest);
		NET_REGISTER(pMgr, MsgID::DS_BASIC_INFO, OnDBBasicInfoResponse);
		NET_REGISTER(pMgr, MsgID::DS_RENAME, OnDBRenameResponse);

		EventManager *emgr = EventManager::GetInstance();
		emgr->RegisterCallBack(LoginEvent::EID_USER_ONLINE, OnEventUserOnline, this);
		emgr->RegisterCallBack(LoginEvent::EID_USER_OUTLINE, OnEventUserOutline, this);
		emgr->RegisterCallBack(LoginEvent::EID_USER_QUIT, OnEventUserQuit, this);
	}

	void PlayerModule::RegisterStorageMessage(StorageManager *pMgr)
	{
		DB_REGISTER(pMgr, MsgID::SD_BASIC_INFO, OnDBBasicInfoRequest);
		DB_REGISTER(pMgr, MsgID::SD_BASIC_SAVE, OnDBBasicSaveRequest);
		DB_REGISTER(pMgr, MsgID::SD_RENAME, OnDBRenameRequest);
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

	void PlayerModule::OnUpdatePerSecond()
	{
		//10秒检测一次全体玩家是否有需要保存的
		static int count = 0;
		if (++count >= 10)
		{
			Int64 now = TimeUtil::GetCurrentSecond();
			count = 0;
			for (PlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
			{
				PlayerBasicData *player = itr->second;
				if (player->GetSaveTimeStamp() > 0 && player->GetSaveTimeStamp() <= now)
				{
					player->SavePlayerData();
				}
			}
		}
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
		::printf("PlayerModule::OnRenameRequest roleid:%lld name:%s\n", item->key.second, req.Name.c_str());

		//名称合法性
		if (!UserInfo::CheckRoleName(req.Name))
		{
			SCRenameResponse res;
			res.Result = 1;
			SendNet(item->key.second, MsgID::SC_RENAME, &res);
		}

		//判断改名消耗(依赖背包模块)

		//DB请求
		SDRenameRequest req2;
		req2.Name = req.Name;
		RequestStorage(item->key.second, MsgID::SD_RENAME, &req2);
	}

	void PlayerModule::OnDBBasicInfoRequest(NetDataItem *item)
	{
		//读取信息
		Int64 rid = item->key.second;
		DSBasicInfoResponse res;
		MySQLWrap *mysql = GetMySQL();
		char sql[128];
		::sprintf(sql, "call sp_select_role(%lld);", rid);
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
		ResponseStorage(rid, MsgID::DS_BASIC_INFO, &res);
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
		SDBasicSaveRequest req;
		req.Unpack(item->data, 0);

		char sql[128];
		MySQLWrap *mysql = GetMySQL();
		::sprintf(sql, "call sp_update_role_basic(%lld,%d,%lld,%d,%d,%d,%d);", 
			item->key.second, req.Level, req.Exp, req.Map, req.PositionX, req.PositionY, req.Direction);
		mysql->Execute(sql);
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
		::sprintf(sql, "call sp_select_role_by_name('%s');", req.Name.c_str());
		auto ret = mysql->Query(sql);
		if (ret->GetRecord())
		{
			Int64 roleid = ret->GetInt64("id");
			if (roleid != 0)
			{
				res.Result = 2;
				ResponseStorage(0, MsgID::DS_RENAME, &res);
				return;
			}
		}
		ret->Clear();

		//保存名称
		::sprintf(sql, "call sp_update_role_name(%lld,'%s');", item->key.second, req.Name.c_str());
		mysql->Execute(sql);

		//回复
		res.Result = 0;
		ResponseStorage(0, MsgID::DS_RENAME, &res);
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
		SendNet(player->GetID(), MsgID::CS_RENAME, &res2);
	}

	void PlayerModule::OnEventUserOnline(int id, EventParam *ep)
	{
		::printf("PlayerModule::OnEventUserOnline user:%s roleid:%lld\n", ep->s1.c_str(), ep->l1);
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
		RequestStorage(player->GetID(), MsgID::SD_BASIC_INFO, &req);
	}

	void PlayerModule::OnEventUserOutline(int id, EventParam *ep)
	{
		::printf("PlayerModule::OnEventUserOutline roleid:%lld\n", ep->l1);
		PlayerMap::iterator itr = m_Players.find(ep->l1);
		if (itr == m_Players.end())
		{
			return;
		}

		//掉线了就保存一下
		itr->second->SavePlayerData();
	}

	void PlayerModule::OnEventUserQuit(int id, EventParam *ep)
	{
		::printf("PlayerModule::OnEventUserQuit roleid:%lld\n", ep->l1);
		PlayerMap::iterator itr = m_Players.find(ep->l1);
		if (itr == m_Players.end())
		{
			return;
		}

		//发送角色销毁事件
		EventParam *ep2 = EventParam::Get(PlayerEvent::EID_PLAYER_DESTROY);
		ep2->p1 = itr->second;
		EventManager::GetInstance()->TriggerEvent(ep2);

		//保存数据，销毁角色
		itr->second->SavePlayerData();
		SAFE_DELETE(itr->second);
		itr = m_Players.erase(itr);
	}
}