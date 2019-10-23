/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: LoginDefine.h
* Summary: 登陆模块定义。
*
* Author: XuXiang
* Date: 2019-10-23 11:43
*******************************************************/

#ifndef __LoginDefine_h__
#define __LoginDefine_h__

namespace XX004
{
	//登陆事件0-99
	enum LoginEvent
	{
		//玩家上线 s1:username l1:roleid
		EID_USER_ONLINE = 1,

		//玩家下线 s1:username l1:roleid
		EID_USER_OUTLINE = 2,

		//玩家退出 s1:username l1:roleid
		EID_USER_QUIT = 3,
	};
}

#endif