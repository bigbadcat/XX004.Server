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
#include <Util/StringUtil.h>
#include <Frame/MainBase.h>
#include <Frame/ServerBase.h>

namespace XX004
{
	RoleInfo::RoleInfo() : ID(0), Prof(0), CreateTime(0), FreeTime(0), Level(0)
	{
	}

	const int UserInfo::ROLE_ID_STAMP_BIT = 1000000;

	Int64 UserInfo::NextRoleStamp = 0;

	const int UserInfo::MAX_ROLE_NUMBER = 5;

	//角色ID格式为：服务器ID+序号(6位) PS:十进制位数
	Int64 UserInfo::GetNewID()
	{
		//若还没有获取本服创建序号则还不能分配角色ID或者已达到上限
		if (NextRoleStamp == 0 || NextRoleStamp >= ROLE_ID_STAMP_BIT)
		{
			return 0;
		}

		ServerBase *server = MainBase::GetCurMain()->GetServer();
		Int64 id = server->GetServerID();
		Int64 uid = id * ROLE_ID_STAMP_BIT + NextRoleStamp++;
		return uid;
	}

	UserInfo::UserInfo(const string& name) : m_Name(name), m_UID(0), m_CurRoleID(0), m_Online(false), m_QuitTimeStamp(0)
	{

	}

	//检测名称字符串
	bool CheckNameChar(char c)
	{
		if (c >= '0' && c <= '9')
		{
			return true;
		}
		if (c >= 'A' && c <= 'Z')
		{
			return true;
		}
		if (c >= 'a' && c <= 'z')
		{
			return true;
		}
		return c == '_';
	}

	bool UserInfo::CheckRoleName(const std::string& name)
	{
		//名称长度
		int width = StringUtil::GetStringWidth(name);
		if (width < 4 || width > 12)
		{
			return false;
		}

		//非法字符
		int index = 0;
		while (index < name.length())
		{
			char c = name[index];
			if (c & 0x80)
			{
				index += 3;		//汉字UTF-8占三个字节(遇到占两个字节的字符编码会不正确)
			}
			else
			{
				//非汉字只能是数字、字母和下划线
				if (CheckNameChar(c))
				{
					++index;
				}
				else
				{
					return false;
				}
			}
		}

		return true;
	}

	RoleInfo* UserInfo::GetRoleInfo(Int64 id)
	{
		RoleInfo *info = NULL;
		for (vector<RoleInfo>::iterator itr = m_RoleInfos.begin(); itr != m_RoleInfos.end(); ++itr)
		{
			if (itr->ID == id)
			{
				info = &(*itr);
				break;
			}
		}
		return info;
	}

	void UserInfo::SetOutline(int delay, bool can_recon)
	{
		m_Online = false;
		m_QuitTimeStamp = TimeUtil::GetCurrentSecond() + delay;
		if (!can_recon)
		{
			m_ReconToken.clear();
		}
	}
}