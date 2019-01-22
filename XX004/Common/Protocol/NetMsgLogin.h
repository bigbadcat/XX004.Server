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

		//登陆的角色信息
		class LoginRoleInfo : public NetMessage
		{
		public:
			LoginRoleInfo();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int64 ID;								//编号
			Int32 Prof;								//职业
			Int64 CreateTime;						//创建时间
			string Name;							//名称
			Int32 Level;							//等级
		};

		//1002:用户登录回复(登陆服->网关)
		class LGLoginResponse : public NetMessage
		{
		public:
			LGLoginResponse();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//登陆结果(0、成功 1、非法账号 2、被冻结)
			string UserName;						//用户名
			Int64 FreeTime;							//解冻时间
			Int32 RoleCount;						//角色数量
			vector<LoginRoleInfo> RoleList;			//角色列表
		};

		//1003:创建角色回复(登陆服->网关)
		class LGCreateRoleResponse : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			LoginRoleInfo Role;						//角色
		};

		//1004:进入游戏请求(客户端->网关)
		class CGEnterGameRequest : public NetMessage
		{
		public:
			CGEnterGameRequest();
			virtual int Unpack(Byte *buffer, int index);

			Int64 RoleID;							//角色编号
		};

		//1005:进入游戏成功(世界端->网关)
		class WGEnterGameSuccess : public NetMessage
		{
		public:
			WGEnterGameSuccess();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int64 RoleID;							//角色编号
		};

		//1006:退出游戏请求(客户端->网关)
		class CGQuitGameRequest : public NetMessage
		{
		};

		//2001:用户登录(网关->登陆服)
		class GLLoginRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
		};

		//2002:回复用户信息
		class DLUserInfoResponse : public NetMessage
		{
		public:
			DLUserInfoResponse();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int64 FreeTime;							//解冻时间
			Int32 RoleCount;						//角色数量
			vector<LoginRoleInfo> RoleList;			//角色列表
		};

		//2003:创建角色请求
		class CLCreateRoleRequest : public NetMessage
		{
		public:
			CLCreateRoleRequest();
			virtual int Unpack(Byte *buffer, int index);

			string UserName;						//用户名
			string RoleName;						//角色名
			Int32 Prof;								//职业
		};

		//2004:进入游戏请求(网关->登陆服)
		class GLEnterGameRequest : public NetMessage
		{
		public:
			GLEnterGameRequest();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int64 RoleID;							//角色编号
		};

		//10001:角色上线
		class LWRoleOnline : public NetMessage
		{
		public:
			LWRoleOnline();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int64 RoleID;							//角色编号
		};

		//10002:角色离线
		class GWRoleOutline : public NetMessage
		{
		public:
			GWRoleOutline();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int64 RoleID;							//角色编号
		};

		//10003:角色退出
		class GWRoleQuit : public NetMessage
		{
		public:
			GWRoleQuit();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int64 RoleID;							//角色编号
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
			LDUserSaveRequest();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int64 CreateTime;						//创建时间
			LoginRoleInfo Role;						//角色(ID为0表示不需要保存角色信息)
		};

		//30001:用户登录回复
		class GCLoginResponse : public NetMessage
		{
		public:
			GCLoginResponse();
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//登陆结果(0、成功 1、非法账号 2、被冻结)
			string UserName;						//用户名
			Int64 FreeTime;							//解冻时间
			Int32 RoleCount;						//角色数量
			vector<LoginRoleInfo> RoleList;			//角色列表
		};

		//30002:创建角色回复
		class GCCreateRoleResponse : public NetMessage
		{
		public:
			virtual int Pack(Byte *buffer, int index);

			LoginRoleInfo Role;						//角色
		};

		//30003:进入游戏失败
		class LCEnterGameFailed : public NetMessage
		{
		public:
			LCEnterGameFailed();
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//结果(0、未知 1、角色不存在 2、被冻结)
			Int64 FreeTime;							//解冻时间
		};

		//30004:进入游戏成功
		class GCEnterGameSuccess : public NetMessage
		{
		public:
			virtual int Pack(Byte *buffer, int index);

			string Token;							//重连令牌
		};

		//30005:退出游戏回复(网关->客户端)
		class GCQuitGameResponse : public NetMessage
		{
		};
	}
}

#endif	//__NetMsgLogin_h__

