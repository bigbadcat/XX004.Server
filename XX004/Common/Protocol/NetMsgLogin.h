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
		//协议号
		namespace MsgID
		{
			const Int32 CS_LOGIN = 1001;						//用户登陆请求
			const Int32 CS_CREATE_ROLE = 1002;					//创建角色请求
			const Int32 CS_ENTER_GAME = 1003;					//进入游戏请求
			const Int32 CS_QUIT_GAME = 1004;					//退出游戏请求
			const Int32 SC_LOGIN = 1401;						//用户登录回复
			const Int32 SC_CREATE_ROLE = 1402;					//创建角色回复
			const Int32 SC_ENTER_GAME = 1403;					//进入游戏回复
			const Int32 SC_QUIT_GAME = 1404;					//退出游戏回复
			const Int32 SD_ROLE_LIST = 1801;					//请求角色列表
			const Int32 DS_ROLE_LIST = 1802;					//回复角色列表
			const Int32 SD_ROLE_ADD = 1803;						//请求添加角色
			const Int32 DS_ROLE_ADD = 1804;						//回复添加角色
		}

		//1001:用户登陆请求
		class CSLoginRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);

			string Token;							//登陆Token
		};

		//1002:创建角色请求
		class CSCreateRoleRequest : public NetMessage
		{
		public:
			CSCreateRoleRequest();
			virtual int Unpack(Byte *buffer, int index);

			string RoleName;						//角色名
			Int32 Prof;								//职业
		};

		//1003:进入游戏请求
		class CSEnterGameRequest : public NetMessage
		{
		public:
			CSEnterGameRequest();
			virtual int Unpack(Byte *buffer, int index);

			Int64 RoleID;							//角色编号
		};

		//1004:退出游戏请求
		class CSQuitGameRequest : public NetMessage
		{
		public:
			CSQuitGameRequest();
			virtual int Unpack(Byte *buffer, int index);

			Int32 Type;								//退出类型(0、关闭游戏 1、返回登陆 2、选择角色)
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
			Int64 FreeTime;							//解冻时间
			string Name;							//名称
			Int32 Level;							//等级
		};

		//1401:用户登录回复
		class SCLoginResponse : public NetMessage
		{
		public:
			SCLoginResponse();
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//登陆结果(0、成功 1、无效Token)
			Int32 RoleCount;						//角色数量
			vector<LoginRoleInfo> RoleList;			//角色列表
		};

		//1402:创建角色回复
		class SCCreateRoleResponse : public NetMessage
		{
		public:
			SCCreateRoleResponse();
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//创建结果(0、成功 1、非法名称 2、非法职业 3、角色已满 4、用户未登陆 5、服务器已满 6、名称已存在)
			LoginRoleInfo Role;						//角色
		};

		//1403:进入游戏回复
		class SCEnterGameResponse : public NetMessage
		{
		public:
			SCEnterGameResponse();
			virtual int Pack(Byte *buffer, int index);

			Int32 Result;							//结果(0、成功 1、角色不存在 2、被冻结 3、未登录)
			Int64 FreeTime;							//解冻时间
		};

		//1404:退出游戏回复
		class SCQuitGameResponse : public NetMessage
		{
		public:
			SCQuitGameResponse();
			virtual int Pack(Byte *buffer, int index);

			Int32 Type;								//退出类型(0、关闭游戏 1、返回登陆 2、选择角色)
		};

		//1801:请求角色列表
		class SDRoleListRequest : public NetMessage
		{
		public:
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
		};

		//1802:回复角色列表
		class DSRoleListResponse : public NetMessage
		{
		public:
			DSRoleListResponse();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int32 RoleCount;						//角色数量
			vector<LoginRoleInfo> RoleList;			//角色列表
		};

		//1803:请求添加角色
		class SDRoleAddRequest : public NetMessage
		{
		public:
			SDRoleAddRequest();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			string RoleName;						//角色名
			Int32 Prof;								//职业
		};

		//1804:回复添加角色
		class DSRoleAddResponse : public NetMessage
		{
		public:
			DSRoleAddResponse();
			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			string UserName;						//用户名
			Int32 Result;							//添加结果(0、成功 3、角色已满 5、服务器已满 6、名称重复)
			LoginRoleInfo Role;						//角色
		};
	}
}

#endif

