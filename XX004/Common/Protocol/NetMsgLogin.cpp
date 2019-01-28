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
		int CGLoginRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

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

		LGLoginResponse::LGLoginResponse() : Result(0), FreeTime(0), RoleCount(0)
		{
		}

		int LGLoginResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Result = DataUtil::ReadInt32(buffer, i, &i);
			UserName = DataUtil::ReadString(buffer, i, &i);
			FreeTime = DataUtil::ReadInt64(buffer, i, &i);
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

		int LGLoginResponse::Pack(Byte *buffer, int index)
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

		//----------------------------------------

		int LGCreateRoleResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			i = Role.Unpack(buffer, i);
			return i;
		}

		int LGCreateRoleResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = Role.Pack(buffer, i);
			return i;
		}

		//----------------------------------------

		CGEnterGameRequest::CGEnterGameRequest() : RoleID(0)
		{
		}

		int CGEnterGameRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			RoleID = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		//----------------------------------------

		WGEnterGameSuccess::WGEnterGameSuccess() : RoleID(0)
		{
		}

		int WGEnterGameSuccess::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			RoleID = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int WGEnterGameSuccess::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt64(buffer, i, RoleID);
			return i;
		}

		//----------------------------------------

		//Empty class - CGQuitGameRequest

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

		DLUserInfoResponse::DLUserInfoResponse() : CreateTime(0), FreeTime(0), RoleCount(0)
		{
		}

		int DLUserInfoResponse::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			CreateTime = DataUtil::ReadInt64(buffer, i, &i);
			FreeTime = DataUtil::ReadInt64(buffer, i, &i);
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

		int DLUserInfoResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt64(buffer, i, CreateTime);
			i = DataUtil::WriteInt64(buffer, i, FreeTime);
			i = DataUtil::WriteInt32(buffer, i, RoleCount);
			for (int j = 0; j < RoleCount; ++j)
			{
				i = RoleList[j].Pack(buffer, i);
			}
			return i;
		}

		//----------------------------------------

		CLCreateRoleRequest::CLCreateRoleRequest() : Prof(0)
		{
		}

		int CLCreateRoleRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			RoleName = DataUtil::ReadString(buffer, i, &i);
			Prof = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		//----------------------------------------

		GLEnterGameRequest::GLEnterGameRequest() : RoleID(0)
		{
		}

		int GLEnterGameRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			RoleID = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int GLEnterGameRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt64(buffer, i, RoleID);
			return i;
		}

		//----------------------------------------

		LWRoleOnline::LWRoleOnline() : RoleID(0)
		{
		}

		int LWRoleOnline::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			RoleID = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int LWRoleOnline::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt64(buffer, i, RoleID);
			return i;
		}

		//----------------------------------------

		GWRoleOutline::GWRoleOutline() : RoleID(0)
		{
		}

		int GWRoleOutline::Unpack(Byte *buffer, int index)
		{
			int i = index;
			RoleID = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int GWRoleOutline::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, RoleID);
			return i;
		}

		//----------------------------------------

		GWRoleQuit::GWRoleQuit() : RoleID(0)
		{
		}

		int GWRoleQuit::Unpack(Byte *buffer, int index)
		{
			int i = index;
			RoleID = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int GWRoleQuit::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, RoleID);
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

		LDUserSaveRequest::LDUserSaveRequest() : CreateTime(0)
		{
		}

		int LDUserSaveRequest::Unpack(Byte *buffer, int index)
		{
			int i = index;
			UserName = DataUtil::ReadString(buffer, i, &i);
			CreateTime = DataUtil::ReadInt64(buffer, i, &i);
			i = Role.Unpack(buffer, i);
			return i;
		}

		int LDUserSaveRequest::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, UserName);
			i = DataUtil::WriteInt64(buffer, i, CreateTime);
			i = Role.Pack(buffer, i);
			return i;
		}

		//----------------------------------------

		GCLoginResponse::GCLoginResponse() : Result(0), FreeTime(0), RoleCount(0)
		{
		}

		int GCLoginResponse::Pack(Byte *buffer, int index)
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

		//----------------------------------------

		int GCCreateRoleResponse::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = Role.Pack(buffer, i);
			return i;
		}

		//----------------------------------------

		LCEnterGameFailed::LCEnterGameFailed() : Result(0), FreeTime(0)
		{
		}

		int LCEnterGameFailed::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Result);
			i = DataUtil::WriteInt64(buffer, i, FreeTime);
			return i;
		}

		//----------------------------------------

		int GCEnterGameSuccess::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, Token);
			return i;
		}

		//----------------------------------------

		//Empty class - GCQuitGameResponse
	}
}

