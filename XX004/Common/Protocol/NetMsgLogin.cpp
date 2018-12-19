﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMsgLogin.cpp
* Summary: 登陆模块协议。
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "NetMsgLogin.h"
#include <iostream>
#include <string>
using namespace std;

namespace XX004
{
	namespace Net
	{
		int CGLoginRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		//----------------------------------------

		int GLLoginRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		int GLLoginRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			return i;
		}

		//----------------------------------------

		int LoginRoleInfo::Unpack(Byte *buffer, int index)
		{
			int i = index;
			ID = DataUtil::ReadInt64(buffer, i, &i);
			Name = DataUtil::ReadString(buffer, i, &i);
			Level = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		int LoginRoleInfo::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, ID);
			i = DataUtil::WriteString(buffer, i, Name);
			i = DataUtil::WriteInt32(buffer, i, Level);
			return i;
		}

		//----------------------------------------

		int DLUserInfoResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			FreeTime = DataUtil::ReadInt64(buffer, i, &i);			
			RoleCount = DataUtil::ReadInt32(buffer, i, &i);
			RoleList.clear();
			for (int j = 0; j < RoleCount; ++j)
			{
				LoginRoleInfo item;
				i = item.Unpack(buffer, i);
				RoleList.push_back(item);
			}
			return i;
		}

		int DLUserInfoResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt64(buffer, i, FreeTime);			
			i = DataUtil::WriteInt32(buffer, i, RoleCount);
			for (int j = 0; j < RoleCount; ++j)
			{
				i = RoleList[j].Pack(buffer, i);
			}
			return i;
		}

		//----------------------------------------

		int LDUserInfoRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		int LDUserInfoRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			return i;
		}

		//----------------------------------------

		int LDUserSaveRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			CreateTime = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int LDUserSaveRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt64(buffer, i, CreateTime);
			return i;
		}

		//----------------------------------------

		int LCLoginResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Result);
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt64(buffer, i, FreeTime);
			i = DataUtil::WriteInt32(buffer, i, RoleCount);
			for (int j = 0; j < RoleCount; ++j)
			{
				i = RoleList[j].Pack(buffer, i);
			}
			return i;
		}
	}
}