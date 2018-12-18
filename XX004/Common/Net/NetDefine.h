/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetDefine.h
* Summary: 网络相关定义定义。
*
* Author: XuXiang
* Date: 2018-11-05 20:04
*******************************************************/

#ifndef __NetDefine_h__
#define __NetDefine_h__

namespace XX004
{
	namespace Net
	{
		//远端类型。
		enum RemoteType
		{
			//未知
			RT_UNKNOW = 0,

			//玩家客户端。
			RT_CLIENT = 1,

			//网关
			RT_GATE = 2,

			//登陆服
			RT_LOGIN = 3,

			//数据服
			RT_DATA = 4,

			//世界服
			RT_WORLD = 5,

			//场景服
			RT_SCENE = 6,

			//区服
			RT_REGION = 7,
		};

#define SAFE_CLOSE_SOCKET(s) if (s!=SOCKET_ERROR){::closesocket(s); s=SOCKET_ERROR;}

		//网络缓冲区大小
		const int NET_BUFFER_SIZE = 8 * 1024;

		//网络数据包最大尺寸 MTU可用大小为1480
		const int NET_PACKAGE_MAX_SIZE = 1024;

		//系统内部起始消息号
		const int NET_MSG_ID_SYSTEM_BEGIN = 0;

		//系统内部结束消息号
		const int NET_MSG_ID_SYSTEM_END = 999;

		//网关起始消息号
		const int NET_MSG_ID_GATE_BEGIN = 1000;

		//网关结束消息号
		const int NET_MSG_ID_GATE_END = 1999;

		//登陆服起始消息号
		const int NET_MSG_ID_LOGIN_BEGIN = 2000;

		//登陆服结束消息号
		const int NET_MSG_ID_LOGIN_END = 2999;

		//世界服起始消息号
		const int NET_MSG_ID_SCENE_BEGIN = 3000;

		//世界服结束消息号
		const int NET_MSG_ID_SCENE_END = 9999;

		//场景服起始消息号
		const int NET_MSG_ID_WORLD_BEGIN = 10000;

		//场景服结束消息号
		const int NET_MSG_ID_WORLD_END = 19999;

		//数据库起始消息号
		const int NET_MSG_ID_DATA_BEGIN = 20000;

		//数据库结束消息号
		const int NET_MSG_ID_DATA_END = 29999;

		//客户端起始消息号
		const int NET_MSG_ID_CLIENT_BEGIN = 30000;

		//客户端结束消息号
		const int NET_MSG_ID_CLIENT_END = 59999;
	}
}

#endif	//__NetDefine_h__