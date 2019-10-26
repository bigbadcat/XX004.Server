/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerDefine.h
* Summary: 玩家基础模块定义。
*
* Author: XuXiang
* Date: 2019-10-25 12:17
*******************************************************/

#ifndef __PlayerDefine_h__
#define __PlayerDefine_h__

namespace XX004
{
	//玩家事件101-199
	enum PlayerEvent
	{
		//玩家创建 p1:player
		EID_PLAYER_CREATE = 101,

		//玩家创建 p1:player
		EID_PLAYER_DESTROY = 102,

		//将玩家数据同步到客户端 p1:player
		EDI_PLAYER_SYNC_CLIENT = 103,
	};
}

#endif