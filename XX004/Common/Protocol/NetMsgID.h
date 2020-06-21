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
	namespace Net
	{
		//网络消息号
		//消息约定规则如下:
		//1、1-999 为系统内部使用。
		//2、千位以上为功能模块标识，每个功能模块有最多999个协议号。
		//3、模块内1-399为客户端请求，401-799为服务器回复和通知，801-999为数据库通信。
		//4、消息号定义到各自协议头文件中，由协议工具自动生成。
		namespace MsgID
		{
			const Int32 REMOTE_IDENTIFY = 1;						//远端标识
			const Int32 INTERNAL_AWAKE = 2;							//内部唤醒
			const Int32 CS_HEART_BEAT = 3;							//心跳请求
			const Int32 SC_HEART_BEAT = 4;							//心跳回复
		}
	}
}

#endif