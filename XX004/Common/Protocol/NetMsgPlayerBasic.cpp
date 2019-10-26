/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMsgPlayerBasic.cpp
* Summary: 玩家基础模块协议。
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "NetMsgPlayerBasic.h"

namespace XX004
{
	namespace Net
	{
		int CSRenameRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Name = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		//----------------------------------------

		AttrInfo::AttrInfo() : Type(0), Value(0)
		{
		}

		int AttrInfo::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Type);
			i = DataUtil::WriteInt64(buffer, i, Value);
			return i;
		}

		//----------------------------------------

		SCPlayerInfoNotify::SCPlayerInfoNotify() : ID(0), Prof(0), Level(0), Exp(0), Power(0), AttrCount(0)
		{
		}

		int SCPlayerInfoNotify::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, Name);
			i = DataUtil::WriteInt64(buffer, i, ID);
			i = DataUtil::WriteInt32(buffer, i, Prof);
			i = DataUtil::WriteInt32(buffer, i, Level);
			i = DataUtil::WriteInt32(buffer, i, Exp);
			i = DataUtil::WriteInt64(buffer, i, Power);
			i = DataUtil::WriteInt32(buffer, i, AttrCount);
			for (int j = 0; j < AttrCount; ++j)
			{
				i = AttrList[j].Pack(buffer, i);
			}
			return i;
		}

		//----------------------------------------

		SCAttrChangeNotify::SCAttrChangeNotify() : Power(0), AttrCount(0)
		{
		}

		int SCAttrChangeNotify::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, Power);
			i = DataUtil::WriteInt32(buffer, i, AttrCount);
			for (int j = 0; j < AttrCount; ++j)
			{
				i = AttrList[j].Pack(buffer, i);
			}
			return i;
		}

		//----------------------------------------

		SCLevelExpNotify::SCLevelExpNotify() : Level(0), Exp(0)
		{
		}

		int SCLevelExpNotify::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Level);
			i = DataUtil::WriteInt32(buffer, i, Exp);
			return i;
		}

		//----------------------------------------

		SCRenameResponse::SCRenameResponse() : Result(0)
		{
		}

		int SCRenameResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Result);
			i = DataUtil::WriteString(buffer, i, Name);
			return i;
		}

		//----------------------------------------

		//Empty class - SDBasicInfoRequest

		//----------------------------------------

		DSBasicInfoResponse::DSBasicInfoResponse() : Prof(0), Level(0), Exp(0), Map(0), PositionX(0), PositionY(0), Direction(0)
		{
		}

		int DSBasicInfoResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Prof = DataUtil::ReadInt32(buffer, i, &i);
			Name = DataUtil::ReadString(buffer, i, &i);
			Level = DataUtil::ReadInt32(buffer, i, &i);
			Exp = DataUtil::ReadInt64(buffer, i, &i);
			Map = DataUtil::ReadInt32(buffer, i, &i);
			PositionX = DataUtil::ReadInt32(buffer, i, &i);
			PositionY = DataUtil::ReadInt32(buffer, i, &i);
			Direction = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		int DSBasicInfoResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Prof);
			i = DataUtil::WriteString(buffer, i, Name);
			i = DataUtil::WriteInt32(buffer, i, Level);
			i = DataUtil::WriteInt64(buffer, i, Exp);
			i = DataUtil::WriteInt32(buffer, i, Map);
			i = DataUtil::WriteInt32(buffer, i, PositionX);
			i = DataUtil::WriteInt32(buffer, i, PositionY);
			i = DataUtil::WriteInt32(buffer, i, Direction);
			return i;
		}

		//----------------------------------------

		SDBasicSaveRequest::SDBasicSaveRequest() : Level(0), Exp(0), Map(0), PositionX(0), PositionY(0), Direction(0)
		{
		}

		int SDBasicSaveRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Level = DataUtil::ReadInt32(buffer, i, &i);
			Exp = DataUtil::ReadInt64(buffer, i, &i);
			Map = DataUtil::ReadInt32(buffer, i, &i);
			PositionX = DataUtil::ReadInt32(buffer, i, &i);
			PositionY = DataUtil::ReadInt32(buffer, i, &i);
			Direction = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		int SDBasicSaveRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Level);
			i = DataUtil::WriteInt64(buffer, i, Exp);
			i = DataUtil::WriteInt32(buffer, i, Map);
			i = DataUtil::WriteInt32(buffer, i, PositionX);
			i = DataUtil::WriteInt32(buffer, i, PositionY);
			i = DataUtil::WriteInt32(buffer, i, Direction);
			return i;
		}

		//----------------------------------------

		int SDRenameRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Name = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		int SDRenameRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, Name);
			return i;
		}

		//----------------------------------------

		DSRenameResponse::DSRenameResponse() : Result(0)
		{
		}

		int DSRenameResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Result = DataUtil::ReadInt32(buffer, i, &i);
			Name = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		int DSRenameResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Result);
			i = DataUtil::WriteString(buffer, i, Name);
			return i;
		}
	}
}

