/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerData.h
* Summary: 玩家数据。
*
* Author: XuXiang
* Date: 2019-08-04 10:17
*******************************************************/

#ifndef __Player_PlayerData_h__
#define __Player_PlayerData_h__

#include <Protocol/NetProtocol.h>
#include <Macro.h>
#include <string>
using namespace std;
using namespace XX004::Net;

namespace XX004
{
	namespace Player
	{
		class PlayerData
		{
		public:
			PlayerData();
			virtual ~PlayerData();

			//初始化玩家
			void Init(DWRoleBaseInfoResponse *res);

			//获取编号
			Int64 GetID()const { return m_ID; }

			//获取职业
			int GetProf(){ return m_Prof; }

			//获取创建时间
			Int64 GetCreateTime() { return m_CreateTime; }

			//获取名称
			const string& GetName()const { return m_Name; }

			//获取等级
			int GetLevel()const { return m_Level; }

			//获取经验
			int GetExp()const { return m_Exp; }

			//获取所在地图
			int GetMap()const { return m_Map; }

			//获取X位置
			int GetPositionX()const { return m_PositionX; }

			//获取Y位置
			int GetPositionY()const { return m_PositionY; }

			//获取朝向
			int GetDirection()const { return m_Direction; }

		private:

			//编号
			Int64 m_ID;

			//职业
			int m_Prof;

			//创建时间
			Int64 m_CreateTime;

			//名称
			string m_Name;

			//等级
			int m_Level;

			//经验
			int m_Exp;

			//所在地图
			int m_Map;

			//X位置
			int m_PositionX;

			//Y位置
			int m_PositionY;

			//朝向
			int m_Direction;
		};
	}
}

#endif