/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerBasicData.cpp
* Summary: 玩家基础数据。
*
* Author: XuXiang
* Date: 2019-10-21 21:02
*******************************************************/

#include "PlayerBasicData.h"
#include "PlayerDefine.h"
#include <Config/BasicModuleConfig.h>
#include <Protocol/NetProtocol.h>
#include <Frame/NetManagerBase.h>
#include <Frame/MainBase.h>
#include <Frame/EventManager.h>
#include <Frame/ModuleBase.h>
#include <Util/TimeUtil.h>
using namespace XX004::Net;

namespace XX004
{
	const int PlayerBasicData::MODEL_ID = 1;

	const int PlayerBasicData::SAVE_GAP = 60 * 3;

	PlayerBasicData::PlayerBasicData() : PlayerDataModel(MODEL_ID), m_ID(0), m_Prof(0), m_Level(0), m_Exp(0), m_Power(0),
		m_PosMap(0), m_PosX(0), m_PosY(0), m_PosDir(0), m_NeedSave(0), m_SaveTimeStamp(0)
	{

	}

	PlayerBasicData::~PlayerBasicData()
	{
	}

	void PlayerBasicData::AddExp(Int64 exp)
	{
		this->UpdateLevelExp(m_Level, m_Exp + exp);
	}

	void PlayerBasicData::UpdateLevelExp(int level, Int64 exp)
	{
		int old_level = m_Level;
		m_Level = level;
		m_Exp = exp;
		m_NeedSave = true;
		if (old_level != m_Level)
		{
			SetSelfDirty();
			this->SavePlayerData();		//升级了就马上保存一次
		}

		//通知客户端
		SCLevelExpNotify notify;
		notify.Level = m_Level;
		notify.Exp = m_Exp;
		SendNet(NetMsgID::SC_LEVEL_EXP, &notify);
	}

	void PlayerBasicData::UpdatePosition(int map, int x, int y, int dir)
	{
		int old_map = m_PosMap;
		m_PosMap = map;
		m_PosX = x;
		m_PosY = y;
		m_PosDir = dir;
		m_NeedSave = true;
		if (old_map != m_PosMap)
		{
			this->SavePlayerData();		//换地图就马上保存一次
		}
	}

	void PlayerBasicData::SetInitMark(int mid, bool mark)
	{
		if (!mark)
		{
			m_InitMark[mid] = false;
			return;
		}

		//检测标记是否存在
		map<int, bool>::iterator itr = m_InitMark.find(mid);
		if (itr == m_InitMark.end())
		{
			return;
		}

		m_InitMark.erase(itr);
		if (m_InitMark.size() == 0)
		{
			//上线需要同步属性
			printf_s("Player init finish. id:(%I64d)\n", m_ID);
			this->m_SaveTimeStamp = TimeUtil::GetCurrentSecond() + SAVE_GAP;
			this->SendToClient();
		}
	}

	void PlayerBasicData::SendNet(int command, Net::NetMessage *msg)
	{
		RemoteKey key(RemoteType::RT_CLIENT, this->m_ID);
		MainBase::GetCurMain()->GetNetManager()->Send(key, command, msg);
	}

	void PlayerBasicData::RebuildAttr()
	{
		//保存旧属性，重算新属性
		Int64 old_power = m_Power;
		AttrMap old_attrs = m_FinalAttrs;
		const AttrMap& attrs = GetAllAttr().GetAttrs();
		m_FinalAttrs = ModuleConfig::GetInstance<BasicModuleConfig>()->CalcFinalAttr(attrs);
		m_Power = ModuleConfig::GetInstance<BasicModuleConfig>()->CalcPower(m_FinalAttrs);

		//对比m_FinalAttrs与old_attrs的差异
		vector<pair<int, Int64> > change_attrs;
		for (int i = 1; i < AttrType::AT_MAX; ++i)
		{
			if (m_FinalAttrs[i] != old_attrs[i])
			{
				change_attrs.push_back(make_pair(i, m_FinalAttrs[i]));
			}
		}

		//通知客户端
		SCAttrChangeNotify notify;
		notify.Power = m_Power;
		notify.AttrCount = (Int32)change_attrs.size();
		for (int i = 0; i < notify.AttrCount; ++i)
		{
			pair<int, Int64>& attr = change_attrs[i];
			AttrInfo info;
			info.Type = attr.first;
			info.Value = attr.second;
			notify.AttrList.push_back(info);
		}
		if (old_power != notify.Power || notify.AttrCount > 0)		//战力和属性必须有一个变化
		{
			SendNet(NetMsgID::SC_ATTR_CHANGE, &notify);
		}		
	}

	void PlayerBasicData::SavePlayerData()
	{
		if (!m_NeedSave)
		{
			return;
		}

		SDBasicSaveRequest req;
		req.Level = m_Level;
		req.Exp = m_Exp;
		req.Map = m_PosMap;
		req.PositionX = m_PosX;
		req.PositionY = m_PosY;
		req.Direction = m_PosDir;
		ModuleBase::RequestStorage(this->m_ID, NetMsgID::SD_BASIC_SAVE_REQ, &req);
		m_NeedSave = false;
		this->m_SaveTimeStamp = TimeUtil::GetCurrentSecond() + SAVE_GAP;
		printf_s("PlayerBasicData::SavePlayerData %I64d\n", m_ID);
	}

	void PlayerBasicData::OnBuildSelfAttr()
	{
		BasicModuleConfig *basic_cfg = ModuleConfig::GetInstance<BasicModuleConfig>();
		m_SelfAttr.Clear();
		m_SelfAttr.AddAttr(basic_cfg->GetProfAttr(m_Prof, m_Level));
	}

	void PlayerBasicData::OnSendToClient()
	{
		//计算属性
		const AttrMap& attrs = GetAllAttr().GetAttrs();
		m_FinalAttrs = ModuleConfig::GetInstance<BasicModuleConfig>()->CalcFinalAttr(attrs);
		m_Power = ModuleConfig::GetInstance<BasicModuleConfig>()->CalcPower(m_FinalAttrs);
		ConfigUtil::PrintAttr(m_FinalAttrs);

		//通知属性
		SCPlayerInfoNotify notify;
		notify.ID = m_ID;
		notify.Name = m_Name;
		notify.Prof = m_Prof;
		notify.Level = m_Level;
		notify.Exp = m_Exp;
		notify.Power = m_Power;
		notify.AttrCount = (Int32)m_FinalAttrs.size();
		for (AttrMap::iterator itr = m_FinalAttrs.begin(); itr != m_FinalAttrs.end(); ++itr)
		{
			AttrInfo info;
			info.Type = itr->first;
			info.Value = itr->second;
			notify.AttrList.push_back(info);
		}
		SendNet(NetMsgID::SC_PLAYER_INFO, &notify);

		//通知进场
		SCSceneEnterNotify notify2;
		notify2.Map = 0;
		notify2.PositionX = 0;
		notify2.PositionY = 0;
		notify2.Direction = 0;
		SendNet(NetMsgID::SC_SCENE_ENTER, &notify2);
	}
}