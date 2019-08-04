/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerManager.h
* Summary: 玩家管理器。
*
* Author: XuXiang
* Date: 2019-08-04 10:16
*******************************************************/

#ifndef __Player_PlayerManager_h__
#define __Player_PlayerManager_h__

#include <Protocol/NetProtocol.h>
#include <Macro.h>
#include <map>
using namespace std;
using namespace XX004::Net;

namespace XX004
{
	namespace Player
	{
		class PlayerData;
		typedef map<Int64, PlayerData*> PlayerMap;

		class PlayerManager
		{
		public:
			PlayerManager();
			virtual ~PlayerManager();

			//添加玩家
			PlayerData* AddPlayer(DWRoleBaseInfoResponse *res);

			//获取玩家，不要缓存返回值，下次使用重新获取
			PlayerData* GetPlayer(Int64 id)const;

			//移除玩家
			void RemovePlayer(Int64 id);

		private:

			PlayerMap m_PlayerDatas;

		};
	}
}

#endif