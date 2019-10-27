/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMsgPlayerBasic.h
* Summary: 玩家基础模块协议。
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __NetMsgPlayerBasic_h__
#define __NetMsgPlayerBasic_h__

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
		//2001:角色改名请求
		class CSRenameRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);

			string Name;							//角色名称
		};

		//属性信息
		class AttrInfo : public NetMessage
		{
		public:
			AttrInfo();
			virtual int Pack(Byte *buffer, int index);

			Int32 Type;								//类型
			Int64 Value;							//值
		};

		//2401:角色信息通知
		class SCPlayerInfoNotify : public NetMessage
		{
		public:
			SCPlayerInfoNotify();
			virtual int Pack(Byte *buffer, int index);

			Int64 ID;								//编号
			string Name;							//名称
			Int32 Prof;								//职业
			Int32 Level;							//等级
			Int64 Exp;								//经验
			Int64 Power;							//战力
			Int32 AttrCount;						//属性数量
			vector<AttrInfo> AttrList;				//属性列表
		};

		//2402:属性变化通知
		class SCAttrChangeNotify : public NetMessage
		{
		public:
			SCAttrChangeNotify();
			virtual int Pack(Byte *buffer, int index);

			Int64 Power;							//战力
			Int32 AttrCount;						//属性数量
			vector<AttrInfo> AttrList;				//属性列表
		};

		//2403:等级经验变化通知
		class SCLevelExpNotify : public NetMessage
		{
		public:
			SCLevelExpNotify();
			virtual int Pack(Byte *buffer, int index);

			Int32 Level;							//等级
			Int64 Exp;								//经验
		};

		//2404:角色改名回复
		class SCRenameResponse : public NetMessage
		{
		public:
			SCRenameResponse();
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//改名结果(0、成功 1、非法名称 2、名称重复 3、道具不足)
			string Name;							//角色名称
		};

		//2801:请求角色基本信息
		class SDBasicInfoRequest : public NetMessage
		{
		};

		//2802:回复角色基本信息
		class DSBasicInfoResponse : public NetMessage
		{
		public:
			DSBasicInfoResponse();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int32 Prof;								//职业
			string Name;							//名称
			Int32 Level;							//等级
			Int64 Exp;								//经验
			Int32 Map;								//地图
			Int32 PositionX;						//X位置
			Int32 PositionY;						//Y位置
			Int32 Direction;						//朝向
		};

		//2803:请求保存角色基本信息
		class SDBasicSaveRequest : public NetMessage
		{
		public:
			SDBasicSaveRequest();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int32 Level;							//等级
			Int64 Exp;								//经验
			Int32 Map;								//地图
			Int32 PositionX;						//X位置
			Int32 PositionY;						//Y位置
			Int32 Direction;						//朝向
		};

		//2804:请求角色改名
		class SDRenameRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string Name;							//角色名称
		};

		//2805:回复角色改名
		class DSRenameResponse : public NetMessage
		{
		public:
			DSRenameResponse();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//改名结果(0、成功 2、名称重复)
			string Name;							//角色名称
		};
	}
}

#endif

