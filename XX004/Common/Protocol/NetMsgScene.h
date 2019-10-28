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
		//协议号
		namespace MsgID
		{
			const Int32 CS_MOVE = 3001;							//移动请求
			const Int32 SC_SCENE_ENTER = 3401;					//进入场景通知
			const Int32 SC_POSTION = 3402;						//位置同步通知
		}

		//3001:移动请求
		class CSMoveRequest : public NetMessage
		{
		public:
			CSMoveRequest();
			virtual int Unpack(Byte *buffer, int index);

			Int32 State;							//状态(0、开始 1、调整 2、结束)
			float PositionX;						//X位置
			float PositionY;						//Y位置
			float Direction;						//移动朝向
		};

		//3401:进入场景通知
		class SCSceneEnterNotify : public NetMessage
		{
		public:
			SCSceneEnterNotify();
			virtual int Pack(Byte *buffer, int index);

			Int32 Map;								//地图
			float PositionX;						//X位置
			float PositionY;						//Y位置
			float Direction;						//朝向
		};

		//3402:位置同步通知
		class SCPostionNotify : public NetMessage
		{
		public:
			SCPostionNotify();
			virtual int Pack(Byte *buffer, int index);

			float PositionX;						//X位置
			float PositionY;						//Y位置
			float Direction;						//朝向
		};
	}
}

#endif

