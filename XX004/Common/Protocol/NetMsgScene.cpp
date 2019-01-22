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
	}
}

