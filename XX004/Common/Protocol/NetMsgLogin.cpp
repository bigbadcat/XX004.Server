/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMsgLogin.cpp
* Summary: 登陆模块协议。
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#include "NetMsgLogin.h"

namespace XX004
{
	namespace Net
	{
		int CSLoginRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Token = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		//----------------------------------------

		CSCreateRoleRequest::CSCreateRoleRequest() : Prof(0)
		{
		}

		int CSCreateRoleRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			RoleName = DataUtil::ReadString(buffer, i, &i);
			Prof = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		//----------------------------------------

		CSEnterGameRequest::CSEnterGameRequest() : RoleID(0)
		{
		}

		int CSEnterGameRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			RoleID = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		//----------------------------------------

		//Empty class - CSQuitGameRequest

		//----------------------------------------

		LoginRoleInfo::LoginRoleInfo() : ID(0), Prof(0), CreateTime(0), Level(0)
		{
		}

		int LoginRoleInfo::Unpack(Byte *buffer, int index)
		{
			int i = index;
			ID = DataUtil::ReadInt64(buffer, i, &i);
			Prof = DataUtil::ReadInt32(buffer, i, &i);
			CreateTime = DataUtil::ReadInt64(buffer, i, &i);
			Name = DataUtil::ReadString(buffer, i, &i);
			Level = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		int LoginRoleInfo::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, ID);
			i = DataUtil::WriteInt32(buffer, i, Prof);
			i = DataUtil::WriteInt64(buffer, i, CreateTime);
			i = DataUtil::WriteString(buffer, i, Name);
			i = DataUtil::WriteInt32(buffer, i, Level);
			return i;
		}

		//----------------------------------------

		SCLoginResponse::SCLoginResponse() : Result(0), RoleCount(0)
		{
		}

		int SCLoginResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Result);
			i = DataUtil::WriteInt32(buffer, i, RoleCount);
			for (int j = 0; j < RoleCount; ++j)
			{
				i = RoleList[j].Pack(buffer, i);
			}
			return i;
		}

		//----------------------------------------

		SCCreateRoleResponse::SCCreateRoleResponse() : Result(0)
		{
		}

		int SCCreateRoleResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Result);
			i = Role.Pack(buffer, i);
			return i;
		}

		//----------------------------------------

		SCEnterGameResponse::SCEnterGameResponse() : Result(0), FreeTime(0)
		{
		}

		int SCEnterGameResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Result);
			i = DataUtil::WriteInt64(buffer, i, FreeTime);
			return i;
		}

		//----------------------------------------

		//Empty class - SCQuitGameResponse

		//----------------------------------------

		int SDRoleListRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		int SDRoleListRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			return i;
		}

		//----------------------------------------

		DSRoleListResponse::DSRoleListResponse() : RoleCount(0)
		{
		}

		int DSRoleListResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			RoleCount = DataUtil::ReadInt32(buffer, i, &i);
			RoleList.clear();
			for (int j = 0; j < RoleCount; ++j)
			{
				LoginRoleInfo _item;
				i = _item.Unpack(buffer, i);
				RoleList.push_back(_item);
			}
			return i;
		}

		int DSRoleListResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt32(buffer, i, RoleCount);
			for (int j = 0; j < RoleCount; ++j)
			{
				i = RoleList[j].Pack(buffer, i);
			}
			return i;
		}

		//----------------------------------------

		SDRoleAddRequest::SDRoleAddRequest() : Prof(0)
		{
		}

		int SDRoleAddRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			RoleName = DataUtil::ReadString(buffer, i, &i);
			Prof = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		int SDRoleAddRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteString(buffer, i, RoleName);
			i = DataUtil::WriteInt32(buffer, i, Prof);
			return i;
		}

		//----------------------------------------

		DSRoleAddResponse::DSRoleAddResponse() : Result(0)
		{
		}

		int DSRoleAddResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			Result = DataUtil::ReadInt32(buffer, i, &i);
			i = Role.Unpack(buffer, i);
			return i;
		}

		int DSRoleAddResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt32(buffer, i, Result);
			i = Role.Pack(buffer, i);
			return i;
		}
	}
}

