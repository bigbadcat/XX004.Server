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
	//消息约定规则如下:
	//1、1-999 为系统内部使用。
	//2、千位以上为功能模块标识，每个功能模块有最多999个协议号。
	//3、模块内1-399为客户端请求，401-799为服务器回复和通知，801-999为数据库通信。
	class NetMsgID
	{
	public:

		//--------------------系统内部--------------------//
		static const Int32 REMOTE_IDENTIFY = 1;						//远端标识

		//--------------------登陆模块--------------------//
		static const Int32 CS_LOGIN_REQ = 1001;						//用户登陆请求
		static const Int32 CS_CREATE_ROLE_REQ = 1002;				//创建角色请求		
		static const Int32 CS_ENTER_GAME_REQ = 1003;				//进入游戏请求
		static const Int32 CS_QUIT_GAME_REQ = 1004;					//退出游戏请求
		static const Int32 SC_LOGIN_RES = 1401;						//用户登陆回复
		static const Int32 SC_CREATE_ROLE_RES = 1402;				//创建角色回复		
		static const Int32 SC_ENTER_GAME_RES = 1403;				//进入游戏回复		
		static const Int32 SC_QUIT_GAME_RES = 1404;					//退出游戏回复
		//----------数据库----------//
		static const Int32 SD_ROLE_LIST_REQ = 1801;					//请求用户信息
		static const Int32 DS_ROLE_LIST_RES = 1802;					//回复用户信息
		static const Int32 SD_ROLE_ADD_REQ = 1803;					//请求添加角色信息		
		static const Int32 DS_ROLE_ADD_RES = 1804;					//回复添加角色信息

		//--------------------场景模块--------------------//
		static const Int32 CS_MOVE_REQ = 2001;						//移动请求
		static const Int32 SC_SCENE_ENTER = 2401;					//进入场景
		static const Int32 SC_POSITION = 2402;						//位置同步		
		//----------数据库----------//
		static const Int32 SD_ROLE_BASE_INFO_REQ = 2801;			//请求角色基本信息	
		static const Int32 DS_ROLE_BASE_INFO_RES = 2802;			//回复角色基本信息	




		//-------------------------------------------备份线-------------------------------------------

		////----------系统内部 begin 0----------

		//static const Int32 REMOTE_IDENTIFY = 1;						//远端标识

		////----------系统内部 end 999----------

		////----------网关 begin 1000----------
		//static const Int32 GATE_BEGIN = 1000;

		//static const Int32 CG_LOGIN_REQ = 1001;						//用户登陆请求(客户端->网关)		
		//static const Int32 LG_LOGIN_RES = 1002;						//用户登录回复(登陆服->网关)
		//static const Int32 LG_CREATE_ROLE_RES = 1003;				//创建角色回复(登陆服->网关)
		//static const Int32 CG_ENTER_GAME_REQ = 1004;				//进入游戏请求(客户端->网关)
		//static const Int32 WG_ENTER_GAME_SUCCESS = 1005;			//进入游戏成功(世界服->网关)
		//static const Int32 CG_QUIT_GAME_REQ = 1006;					//退出游戏请求(客户端->网关)

		//static const Int32 GATE_END = 1999;
		////----------网关 end 1999----------

		////----------登陆服 begin 2000----------
		//static const Int32 LOGIN_BEGIN = 2000;						

		//static const Int32 GL_LOGIN_REQ = 2001;						//用户登录(网关->登陆服)	
		//static const Int32 DL_USER_INFO_RES = 2002;					//回复用户信息
		//static const Int32 CL_CREATE_ROLE_REQ = 2003;				//创建角色请求
		//static const Int32 GL_ENTER_GAME_REQ = 2004;				//进入游戏请求(网关->登陆服)
		//static const Int32 DL_ROLE_STAMP_RES = 2005;				//回复登陆服角色创建序号
		//static const Int32 DL_ROLE_ADD_RES = 2006;					//回复登陆服角色添加

		//static const Int32 LOGIN_END = 2999;
		////----------登陆服 end 2999----------

		////----------场景服 begin 3000----------
		//static const Int32 SCENE_BEGIN = 3000;						

		//static const Int32 WS_ROLE_ENTER = 3001;					//角色进场
		//static const Int32 CS_MOVE_REQ = 3002;						//移动请求

		//static const Int32 SCENE_END = 9999;
		////----------场景服 end 9999----------

		////----------世界服 begin 10000----------
		//static const Int32 WORLD_BEGIN = 10000;						

		//static const Int32 LW_ROLE_ONLINE = 10001;					//角色上线
		//static const Int32 GW_ROLE_OUTLINE = 10002;					//角色离线
		//static const Int32 GW_ROLE_QUIT = 10003;					//角色退出
		//static const Int32 DW_ROLE_BASE_INFO_REQ = 10004;			//回复角色基本信息

		//static const Int32 WORLD_END = 19999;
		////----------世界服 end 19999----------

		////----------数据库 begin 20000----------
		//static const Int32 DATA_BEGIN = 20000;

		//static const Int32 LD_USER_INFO_REQ = 20001;				//请求用户信息		
		//static const Int32 LD_USER_SAVE_REQ = 20002;				//请求保存用户信息
		//static const Int32 LD_ROLE_STAMP_REQ = 20003;				//请求服务器场景序号
		//static const Int32 LD_ROLE_ADD_REQ = 20004;					//请求添加角色信息
		//static const Int32 WD_ROLE_BASE_INFO_REQ = 20005;			//请求角色基本信息

		//static const Int32 DATA_END = 29999;
		////----------数据库 end 29999----------

		////----------客户端 begin 30000----------
		//static const Int32 CLIENT_BEGIN = 30000;					

		//static const Int32 GC_LOGIN_RES = 30001;					//用户登录回复
		//static const Int32 GC_CREATE_ROLE_RES = 30002;				//创建角色回复
		//static const Int32 LC_ENTER_GAME_FAILED = 30003;			//进入游戏失败
		//static const Int32 GC_ENTER_GAME_SUCCESS = 30004;			//进入游戏成功
		//static const Int32 GC_QUIT_GAME_RES = 30005;				//退出游戏回复

		//static const Int32 WC_SCENE_ENTER = 30021;					//进入场景
		//static const Int32 WC_POSITION = 30022;						//位置同步

		//static const Int32 CLIENT_END = 59999;
		////----------客户端 end 59999----------

	};
}

#endif