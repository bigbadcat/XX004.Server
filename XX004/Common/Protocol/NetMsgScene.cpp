/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMsgScene.cpp
* Summary: 场景模块协议。
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "NetMsgScene.h"

namespace XX004
{
	namespace Net
	{
		WSRoleEnter::WSRoleEnter() : RoleID(0), Map(0), PositionX(0), PositionY(0), Direction(0)
		{
		}

		int WSRoleEnter::Unpack(Byte *buffer, int index)
		{
			int i = index;
			RoleID = DataUtil::ReadInt64(buffer, i, &i);
			Map = DataUtil::ReadInt32(buffer, i, &i);
			PositionX = DataUtil::ReadFloat(buffer, i, &i);
			PositionY = DataUtil::ReadFloat(buffer, i, &i);
			Direction = DataUtil::ReadFloat(buffer, i, &i);
			return i;
		}

		int WSRoleEnter::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, RoleID);
			i = DataUtil::WriteInt32(buffer, i, Map);
			i = DataUtil::WriteFloat(buffer, i, PositionX);
			i = DataUtil::WriteFloat(buffer, i, PositionY);
			i = DataUtil::WriteFloat(buffer, i, Direction);
			return i;
		}

		//----------------------------------------

		CSMoveRequest::CSMoveRequest() : State(0), PositionX(0), PositionY(0), Direction(0)
		{
		}

		int CSMoveRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			State = DataUtil::ReadInt32(buffer, i, &i);
			PositionX = DataUtil::ReadFloat(buffer, i, &i);
			PositionY = DataUtil::ReadFloat(buffer, i, &i);
			Direction = DataUtil::ReadFloat(buffer, i, &i);
			return i;
		}

		//----------------------------------------

		WCSceneEnterNotify::WCSceneEnterNotify() : Map(0), PositionX(0), PositionY(0), Direction(0)
		{
		}

		int WCSceneEnterNotify::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Map);
			i = DataUtil::WriteFloat(buffer, i, PositionX);
			i = DataUtil::WriteFloat(buffer, i, PositionY);
			i = DataUtil::WriteFloat(buffer, i, Direction);
			return i;
		}

		//----------------------------------------

		WCPostionNotify::WCPostionNotify() : PositionX(0), PositionY(0), Direction(0)
		{
		}

		int WCPostionNotify::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteFloat(buffer, i, PositionX);
			i = DataUtil::WriteFloat(buffer, i, PositionY);
			i = DataUtil::WriteFloat(buffer, i, Direction);
			return i;
		}
	}
}

