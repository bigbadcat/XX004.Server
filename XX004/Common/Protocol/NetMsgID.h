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

		static const Int32 CG_LOGIN_USER_REQ = 1001;					//用户登陆请求(客户端->网关)		
		static const Int32 LG_LOGIN_USER_RES = 1002;					//用户登录回复(登陆服->网关)		

		//----------网关 end 1999----------

		//----------登陆服 begin 2000----------

		static const Int32 GL_LOGIN_USER_REQ = 2001;					//用户登录(网关->登陆服)	
		static const Int32 DL_LOGIN_USER_RES = 2002;					//回复用户信息

		//----------登陆服 end 2999----------

		//----------世界服 begin 3000----------

		//----------世界服 end 9999----------

		//----------场景服 begin 10000----------

		//----------场景服 end 19999----------

		//----------数据库 begin 20000----------

		static const Int32 LD_LOGIN_USER_REQ = 20001;					//请求用户信息		
		static const Int32 LD_USER_SAVE_REQ = 20002;					//请求保存用户信息

		//----------数据库 end 29999----------

		//----------客户端 begin 30000----------

		static const Int32 GC_LOGIN_USER_RES = 30001;					//用户登录回复

		//----------客户端 end 59999----------

	};
}

#endif	//__NetMsgID_h__