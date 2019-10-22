﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerBasicData.h
* Summary: 玩家基础数据。
*
* Author: XuXiang
* Date: 2019-10-21 21:02
*******************************************************/

#ifndef __PlayerBasicData_h__
#define __PlayerBasicData_h__

#include "PlayerDataModel.h"

namespace XX004
{
	//玩家部分的数据都继承此类
	class PlayerBasicData: public PlayerDataModel
	{
	public:
		PlayerBasicData();
		virtual ~PlayerBasicData();

		static const int MODEL_ID;

		//获取用户名
		inline const string& GetUser()const { return m_User; }

		//获取角色名称
		inline const string& GetName()const { return m_Name; }

		//获取角色编号
		inline Int64 GetID()const { return m_ID; }

		//获取职业
		inline int GetProf()const { return m_Prof; }

		//获取等级
		inline int GetLevel()const { return m_Level; }

		//获取经验
		inline Int64 GetExp()const { return m_Exp; }

		//获取战力
		inline Int64 GetPower()const { return m_Power; }

	private:

		//用户名
		string m_User;

		//名称
		string m_Name;

		//编号
		Int64 m_ID;

		//职业
		int m_Prof;

		//等级
		int m_Level;

		//经验
		Int64 m_Exp;

		//战力
		Int64 m_Power;
	};
}

#endif