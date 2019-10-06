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
	//登陆的角色信息
	struct RoleInfo
	{
		RoleInfo();

		Int64 ID;								//编号
		Int32 Prof;								//职业
		Int64 CreateTime;						//创建时间
		string Name;							//名称
		Int32 Level;							//等级
	};

	class UserInfo
	{
	public:
		UserInfo(const string& name);

		//角色ID序号占位
		static const int ROLE_ID_STAMP_BIT;

		//下一个角色创建标记
		static Int64 NextRoleStamp;

		//获取新的角色ID
		static Int64 GetNewID();

		//最大角色数量
		static const int MAX_ROLE_NUMBER;

		//检测角色名称是否合法
		static bool CheckRoleName(const std::string& name);

		//获取名称
		inline const string& GetName()const { return m_Name; }

		//获取连接标识
		inline const UInt64 GetUID()const { return m_UID; }
		//设置连接标识
		inline void SetUID(UInt64 uid) { m_UID = uid; }

		//获取角色列表
		vector<RoleInfo>& GetRoleInfos() { return m_RoleInfos; }

		//添加角色信息
		void AddRoleInfo(const RoleInfo& info) { m_RoleInfos.push_back(info); }

		//获取角色信息(外部不要缓存返回结果)
		RoleInfo* GetRoleInfo(Int64 id);

	private:

		//角色名称
		string m_Name;

		//连接标识
		UInt64 m_UID;

		//角色信息
		vector<RoleInfo> m_RoleInfos;
	};
	typedef map<string, UserInfo*> UserInfoMap;
}

#endif