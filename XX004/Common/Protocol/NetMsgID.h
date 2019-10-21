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
	};
}

#endif