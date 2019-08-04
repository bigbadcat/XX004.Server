/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerData.cpp
* Summary: 玩家数据。
*
* Author: XuXiang
* Date: 2019-08-04 10:17
*******************************************************/

#include "PlayerData.h"
#include <MainBase.h>
#include <NetManagerBase.h>

namespace XX004
{
	namespace Player
	{
		PlayerData::PlayerData() : m_ID(0), m_Prof(0), m_CreateTime(0), m_Level(0), m_Exp(0), m_Map(0), m_PositionX(0), m_PositionY(0), m_Direction(0)
		{
		}

		PlayerData::~PlayerData()
		{
		}

		void PlayerData::Init(DWRoleBaseInfoResponse *res)
		{
			m_UserName = res->UserName;
			m_ID = res->ID;
			m_Prof = res->Prof;
			m_CreateTime = res->CreateTime;
			m_Name = res->Name;
			m_Level = res->Level;
			m_Exp = res->Exp;
			m_Map = res->Map;
			m_PositionX = res->PositionX;
			m_PositionY = res->PositionY;
			m_Direction = res->Direction;
		}

		void PlayerData::Send(int command, NetMessage *msg)
		{
			MainBase::GetCurMain()->GetNetManager()->Send(RemoteKey(RemoteType::RT_GATE, m_ID), command, msg);
		}
	}
}