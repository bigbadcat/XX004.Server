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
		//2001:移动请求
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

		//2401:进入场景通知
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

		//2402:位置同步通知
		class SCPostionNotify : public NetMessage
		{
		public:
			SCPostionNotify();
			virtual int Pack(Byte *buffer, int index);

			float PositionX;						//X位置
			float PositionY;						//Y位置
			float Direction;						//朝向
		};

		//2801:请求角色基本信息
		class SDRoleBaseInfoRequest : public NetMessage
		{
		public:
			SDRoleBaseInfoRequest();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int64 ID;								//编号
		};

		//2802:回复角色基本信息
		class DSRoleBaseInfoResponse : public NetMessage
		{
		public:
			DSRoleBaseInfoResponse();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int64 ID;								//编号
			Int32 Prof;								//职业
			Int64 CreateTime;						//创建时间
			string Name;							//名称
			Int32 Level;							//等级
			Int64 Exp;								//经验
			Int32 Map;								//地图
			Int32 PositionX;						//X位置
			Int32 PositionY;						//Y位置
			Int32 Direction;						//朝向
		};
	}
}

#endif

