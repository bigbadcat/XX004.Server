﻿/*******************************************************
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

#include "../Macro.h"
#if defined(WIN)
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <execinfo.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/un.h>
#endif

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

			//中控
			RT_MASTER = 2,

			//跨服
			RT_GAME = 3,

			//跨服
			RT_REGION = 4,
		};

		//远端唯一标识。
		typedef std::pair<int, Int64> RemoteKey;

#if defined(WIN)
		typedef SOCKET socket_t;
		typedef int socklen_t;
#define SAFE_CLOSE_SOCKET(s) if (s!=SOCKET_ERROR){::closesocket(s); s=SOCKET_ERROR;}
#define GET_LAST_ERROR() WSAGetLastError()
#else
#define SOCKET_ERROR (-1)
		typedef int socket_t;
#define SAFE_CLOSE_SOCKET(s) if (s!=SOCKET_ERROR){::close(s); s=SOCKET_ERROR;}
#define GET_LAST_ERROR() (errno)
#endif

		//设置socket是否为非阻塞模式
		extern bool socket_set_nonblocking(socket_t s, bool on);

		//网络缓冲区大小
		const int NET_BUFFER_SIZE = 8 * 1024;

		//网络数据包最大尺寸 MTU可用大小为1480
		const int NET_PACKAGE_MAX_SIZE = 1024;
	}
}

#endif