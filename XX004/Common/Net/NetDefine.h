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
		//网络缓冲区大小
		const int NET_BUFFER_SIZE = 8 * 1024;

		//网络数据包最大尺寸 MTU可用大小为1480
		const int NET_PACKAGE_MAX_SIZE = 1024;

#define SAFE_CLOSE_SOCKET(s) if (s!=SOCKET_ERROR){::closesocket(s); s=SOCKET_ERROR;}
	}
}

#endif	//__NetDefine_h__