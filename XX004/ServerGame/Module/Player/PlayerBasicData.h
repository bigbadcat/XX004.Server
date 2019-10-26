/*******************************************************
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
	namespace Net
	{
		class NetMessage;
	}

	//玩家部分的数据都继承此类
	class PlayerBasicData: public PlayerDataModel
	{
		friend class PlayerModule;
	public:
		PlayerBasicData();
		virtual ~PlayerBasicData();

		static const int MODEL_ID;

		//获取用户名
		inline const string& GetUser()const { return m_User; }

		//设置用户名
		inline void SetUser(const string& user) { m_User = user; }

		//获取角色名称
		inline const string& GetName()const { return m_Name; }

		//设置名称
		inline void SetName(const string& name) { m_Name = name; }

		//获取角色编号
		inline Int64 GetID()const { return m_ID; }

		//设置角色编号
		inline void SetID(Int64 id) { m_ID = id; }

		//获取职业
		inline int GetProf()const { return m_Prof; }

		//获取等级
		inline int GetLevel()const { return m_Level; }

		//获取经验
		inline Int64 GetExp()const { return m_Exp; }

		//获取战力
		inline Int64 GetPower()const { return m_Power; }

		//获取所在地图
		inline Int64 GetPosMap()const { return m_PosMap; }

		//获取X坐标
		inline Int64 GetPosX()const { return m_PosX; }

		//获取Y坐标
		inline Int64 GetPosY()const { return m_PosY; }

		//获取朝向
		inline Int64 GetPosDir()const { return m_PosDir; }

		//添加经验
		void AddExp(Int64 exp);

		//更新等级经验
		void UpdateLevelExp(int level, Int64 exp);

		//更新位置
		void UpdatePosition(int map, int x, int y, int dir);

		//设置初始化标记 mid:模块编号 mark:false表示添加，true表示完成
		void SetInitMark(int mid, bool mark = false);

		//发送网络消息
		void SendNet(int command, Net::NetMessage *msg);

		//重新生成属性，并将结果同步客户端
		void RebuildAttr();

	protected:

		//需要重建自身属性
		virtual void OnBuildSelfAttr();

		//同步客户端
		virtual void OnSendToClient();

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

		//所在地图
		int m_PosMap;

		//X坐标
		int m_PosX;

		//Y坐标
		int m_PosY;

		//朝向
		int m_PosDir;

		//初始化标记 key:模块编号
		map<int, bool> m_InitMark;

		//最终属性
		map<int, Int64> m_FinalAttrs;
	};
}

#endif