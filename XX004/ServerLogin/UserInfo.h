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
#include <Protocol/NetProtocol.h>
using namespace std;
using namespace XX004::Net;

namespace XX004
{
	class UserInfo
	{
	public:
		UserInfo(const string& name);

		//角色ID序号占位
		static const int ROLE_ID_STAMP_BIT = 100000000;

		//角色ID服务器占位
		static const int ROLE_ID_SERVER_BIT = 10000;

		//下一个角色创建标记
		static Int64 NextRoleStamp;

		//获取新的角色ID
		static Int64 GetNewID();

		//获取名称
		inline const string& GetName()const { return m_Name; }

		//获取解封时间
		inline Int64 GetFreeTime()const { return m_FreeTime; }
		//设置解封时间
		inline void SetFreeTime(Int64 ft) { m_FreeTime = ft; }

		//获取角色列表
		vector<LoginRoleInfo>& GetRoleInfos() { return m_RoleInfos; }
		//添加角色信息
		void AddRoleInfo(const LoginRoleInfo& info) { m_RoleInfos.push_back(info); }

	private:

		//角色名称
		string m_Name;

		//解冻时间
		Int64 m_FreeTime;

		//角色信息
		vector<LoginRoleInfo> m_RoleInfos;
	};
	typedef map<string, UserInfo*> UserInfoMap;
}

#endif