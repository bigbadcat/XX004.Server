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

		SCSceneEnterNotify::SCSceneEnterNotify() : Map(0), PositionX(0), PositionY(0), Direction(0)
		{
		}

		int SCSceneEnterNotify::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Map);
			i = DataUtil::WriteFloat(buffer, i, PositionX);
			i = DataUtil::WriteFloat(buffer, i, PositionY);
			i = DataUtil::WriteFloat(buffer, i, Direction);
			return i;
		}

		//----------------------------------------

		SCPostionNotify::SCPostionNotify() : PositionX(0), PositionY(0), Direction(0)
		{
		}

		int SCPostionNotify::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteFloat(buffer, i, PositionX);
			i = DataUtil::WriteFloat(buffer, i, PositionY);
			i = DataUtil::WriteFloat(buffer, i, Direction);
			return i;
		}

		//----------------------------------------

		SDRoleBaseInfoRequest::SDRoleBaseInfoRequest() : ID(0)
		{
		}

		int SDRoleBaseInfoRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			ID = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int SDRoleBaseInfoRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, ID);
			return i;
		}

		//----------------------------------------

		DSRoleBaseInfoResponse::DSRoleBaseInfoResponse() : ID(0), Prof(0), CreateTime(0), Level(0), Exp(0), Map(0), PositionX(0), PositionY(0), Direction(0)
		{
		}

		int DSRoleBaseInfoResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			ID = DataUtil::ReadInt64(buffer, i, &i);
			Prof = DataUtil::ReadInt32(buffer, i, &i);
			CreateTime = DataUtil::ReadInt64(buffer, i, &i);
			Name = DataUtil::ReadString(buffer, i, &i);
			Level = DataUtil::ReadInt32(buffer, i, &i);
			Exp = DataUtil::ReadInt64(buffer, i, &i);
			Map = DataUtil::ReadInt32(buffer, i, &i);
			PositionX = DataUtil::ReadInt32(buffer, i, &i);
			PositionY = DataUtil::ReadInt32(buffer, i, &i);
			Direction = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		int DSRoleBaseInfoResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, ID);
			i = DataUtil::WriteInt32(buffer, i, Prof);
			i = DataUtil::WriteInt64(buffer, i, CreateTime);
			i = DataUtil::WriteString(buffer, i, Name);
			i = DataUtil::WriteInt32(buffer, i, Level);
			i = DataUtil::WriteInt64(buffer, i, Exp);
			i = DataUtil::WriteInt32(buffer, i, Map);
			i = DataUtil::WriteInt32(buffer, i, PositionX);
			i = DataUtil::WriteInt32(buffer, i, PositionY);
			i = DataUtil::WriteInt32(buffer, i, Direction);
			return i;
		}
	}
}

