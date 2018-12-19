/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMsgLogin.h
* Summary: 登陆模块协议。
*
* Author: AutoBuild
* Date: --/--
*******************************************************/

#ifndef __NetMsgLogin_h__
#define __NetMsgLogin_h__

#include "../Macro.h"
#include "../Net/NetMessage.h"
#include "../Util/DataUtil.h"
#include <string>
#include <vector>
using namespace std;

namespace XX004
{
	namespace Net
	{
		//1001:登陆请求(客户端->网关)
		class CGLoginRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);

			string UserName;						//用户名
		};

		//2001:用户登录(网关->登陆服)
		class GLLoginRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
		};

		//登陆的角色信息
		class LoginRoleInfo : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int64 ID;								//编号
			string Name;							//名称
			Int32 Level;							//等级
		};

		//2002:回复用户信息
		class DLUserInfoResponse : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int64 FreeTime;							//解冻时间
			Int32 RoleCount;						//角色数量
			vector<LoginRoleInfo> RoleList;			//角色列表
		};

		//20001:请求用户信息
		class LDUserInfoRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
		};

		//20002:请求保存用户信息
		class LDUserSaveRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int64 CreateTime;						//创建时间
		};

		//30001:用户登录回复
		class LCLoginResponse : public NetMessage
		{
		public:
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//登陆结果(0、成功 1、非法账号 2、被冻结)
			string UserName;						//用户名
			Int64 FreeTime;							//解冻时间
			Int32 RoleCount;						//角色数量
			vector<LoginRoleInfo> RoleList;			//角色列表
		};
	}
}

#endif	//__NetMsgLogin_h__