/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerManager.cpp
* Summary: 玩家管理器。
*
* Author: XuXiang
* Date: 2019-08-04 10:16
*******************************************************/

#include "PlayerManager.h"
#include "PlayerData.h"

namespace XX004
{
	namespace Player
	{
		PlayerManager::PlayerManager()
		{

		}

		PlayerManager::~PlayerManager()
		{
			SAFE_DELETE_MAP(m_PlayerDatas);
		}

		PlayerData* PlayerManager::AddPlayer(DWRoleBaseInfoResponse *res)
		{
			PlayerMap::iterator itr = m_PlayerDatas.find(res->ID);
			if (itr != m_PlayerDatas.end())
			{
				SAFE_DELETE(itr->second);
				itr = m_PlayerDatas.erase(itr);
			}

			PlayerData *player = new PlayerData();
			player->Init(res);
			m_PlayerDatas.insert(PlayerMap::value_type(player->GetID(), player));
			return player;
		}

		PlayerData* PlayerManager::GetPlayer(Int64 id)const
		{
			PlayerMap::const_iterator itr = m_PlayerDatas.find(id);
			return itr == m_PlayerDatas.cend() ? NULL : itr->second;
		}

		void PlayerManager::RemovePlayer(Int64 id)
		{
			PlayerMap::iterator itr = m_PlayerDatas.find(id);
			if (itr != m_PlayerDatas.end())
			{
				SAFE_DELETE(itr->second);
				itr = m_PlayerDatas.erase(itr);
			}
		}
	}
}