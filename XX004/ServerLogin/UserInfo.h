/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: UserInfo.h
* Summary: 登陆的用户信息。
*
* Author: XuXiang
* Date: 2019-03-13 19:58
*******************************************************/

#ifndef __UserInfo_h__
#define __UserInfo_h__

#include <map>
#include <vector>
#include <string>
#include <Macro.h>
using namespace std;

namespace XX004
{
	//角色信息
	class UserRoleInfo
	{
	public:
		UserRoleInfo();

		//获取新的角色ID
		static Int64 GetNewID();

		//获取角色编号
		inline Int64 GetID()const { return m_ID; }
		//设置角色编号
		inline void SetID(Int64 id) { m_ID = id; }

		//获取职业
		inline Int32 GetProf()const { return m_Prof; }
		//设置职业
		inline void SetProf(int prof) { m_Prof = prof; }

		//获取创建时间
		inline Int64 GetCreateTime()const { return m_CreateTime; }
		//设置创建时间
		inline void SetCreateTime(Int64 ct) { m_CreateTime = ct; }

		//获取名称
		inline const string& GetName()const { return m_Name; }
		//设置名称
		inline void SetName(const string& name) { m_Name = name; }

		//获取等级
		inline int GetLevel()const { return m_Level; }
		//设置等级
		inline void SetLevel(int level) { m_Level = level; }

	private:
		//角色编号
		Int64 m_ID;

		//职业
		int m_Prof;

		//创建时间
		Int64 m_CreateTime;

		//名称
		string m_Name;

		//等级
		int m_Level;
	};
	
	class UserInfo
	{
	public:
		UserInfo(const string& name);

		//获取名称
		inline const string& GetName()const { return m_Name; }

		//获取解封时间
		inline Int64 GetFreeTime()const { return m_FreeTime; }
		//设置解封时间
		inline void SetFreeTime(Int64 ft) { m_FreeTime = ft; }

		//获取角色列表
		const vector<UserRoleInfo>& GetRoleInfos()const { return m_RoleInfos; }
		//添加角色信息
		void AddRoleInfo(const UserRoleInfo& info) { m_RoleInfos.push_back(info); }

	private:
		//角色名称
		string m_Name;

		//解冻时间
		Int64 m_FreeTime;

		//角色信息
		vector<UserRoleInfo> m_RoleInfos;
	};
	typedef map<string, UserInfo*> UserInfoMap;
}

#endif