/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: UserInfo.cpp
* Summary: 登陆的用户信息。
*
* Author: XuXiang
* Date: 2019-03-13 19:58
*******************************************************/

#include "UserInfo.h"
#include <Util/TimeUtil.h>
#include <MainBase.h>
#include <ServerBase.h>

namespace XX004
{
	Int64 UserInfo::NextRoleStamp = 0;

	//角色ID格式为：服务器组(2位)+服务器ID(4位)+序号(8位) PS:十进制位数
	Int64 UserInfo::GetNewID()
	{
		//若还没有获取本服创建序号则还不能分配角色ID或者已达到上限
		if (NextRoleStamp == 0 || NextRoleStamp >= ROLE_ID_STAMP_BIT)
		{
			return 0;
		}

		ServerBase *server = MainBase::GetCurMain()->GetServe();
		Int64 group = server->GetServerGroup();
		Int64 id = server->GetServerID();
		Int64 uid = 0;
		uid += group * ROLE_ID_STAMP_BIT * ROLE_ID_SERVER_BIT;
		uid += id * ROLE_ID_STAMP_BIT;
		uid += NextRoleStamp++;

		return uid;
	}

	UserInfo::UserInfo(const string& name) : m_Name(name), m_FreeTime(0)
	{

	}

	LoginRoleInfo* UserInfo::GetRoleInfo(Int64 id)
	{
		LoginRoleInfo *info = NULL;
		for (vector<LoginRoleInfo>::iterator itr = m_RoleInfos.begin(); itr != m_RoleInfos.end(); ++itr)
		{
			if (itr->ID == id)
			{
				info = &(*itr);
				break;
			}
		}
		return info;
	}
}