/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMsgScene.h
* Summary: 场景模块协议。
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __NetMsgScene_h__
#define __NetMsgScene_h__

#include "../Macro.h"
#include "../Net/NetMessage.h"
#include "../Util/DataUtil.h"
#include <string>
#include <vector>
using namespace std;

namespace XX004
{
	namespace Net
	{
		//30021:进入场景
		class WCSceneEnterNotify : public NetMessage
		{
		public:
			WCSceneEnterNotify();
			virtual int Pack(Byte *buffer, int index);

			Int32 Map;								//地图
			float PositionX;						//X位置
			float PositionY;						//Y位置
			float Direction;						//朝向
		};
	}
}

#endif

