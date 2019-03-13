/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMsgID.h
* Summary: 协议定义。
*
* Author: XuXiang
* Date: 2018-12-17 20:25
*******************************************************/

#ifndef __NetMsgID_h__
#define __NetMsgID_h__

#include "../Macro.h"

namespace XX004
{
	//网络消息号
	class NetMsgID
	{
	public:

		//----------系统内部 begin 0----------

		static const Int32 REMOTE_IDENTIFY = 1;						//远端标识

		//----------系统内部 end 999----------

		//----------网关 begin 1000----------

		static const Int32 CG_LOGIN_REQ = 1001;						//用户登陆请求(客户端->网关)		
		static const Int32 LG_LOGIN_RES = 1002;						//用户登录回复(登陆服->网关)
		static const Int32 LG_CREATE_ROLE_RES = 1003;				//创建角色回复(登陆服->网关)
		static const Int32 CG_ENTER_GAME_REQ = 1004;				//进入游戏请求(客户端->网关)
		static const Int32 WG_ENTER_GAME_SUCCESS = 1005;			//进入游戏成功(世界服->网关)
		static const Int32 CG_QUIT_GAME_REQ = 1006;					//退出游戏请求(客户端->网关)

		//----------网关 end 1999----------

		//----------登陆服 begin 2000----------

		static const Int32 GL_LOGIN_REQ = 2001;						//用户登录(网关->登陆服)	
		static const Int32 DL_USER_INFO_RES = 2002;					//回复用户信息
		static const Int32 CL_CREATE_ROLE_REQ = 2003;				//创建角色请求
		static const Int32 GL_ENTER_GAME_REQ = 2004;				//进入游戏请求(网关->登陆服)

		//----------登陆服 end 2999----------

		//----------场景服 begin 3000----------

		//----------场景服 end 9999----------

		//----------世界服 begin 10000----------

		static const Int32 LW_ROLE_ONLINE = 10001;					//角色上线
		static const Int32 GW_ROLE_OUTLINE = 10002;					//角色离线
		static const Int32 GW_ROLE_QUIT = 10003;					//角色退出

		//----------世界服 end 19999----------

		//----------数据库 begin 20000----------

		static const Int32 LD_USER_INFO_REQ = 20001;				//请求用户信息		
		static const Int32 LD_USER_SAVE_REQ = 20002;				//请求保存用户信息

		//----------数据库 end 29999----------

		//----------客户端 begin 30000----------

		static const Int32 GC_LOGIN_RES = 30001;					//用户登录回复
		static const Int32 GC_CREATE_ROLE_RES = 30002;				//创建角色回复
		static const Int32 LC_ENTER_GAME_FAILED = 30003;			//进入游戏失败
		static const Int32 LC_ENTER_GAME_SUCCESS = 30004;			//进入游戏成功
		static const Int32 CG_QUIT_GAME_RES = 30005;				//退出游戏回复

		static const Int32 LC_SCENE_ENTER = 30021;					//进入场景



		//----------客户端 end 59999----------

	};
}

#endif