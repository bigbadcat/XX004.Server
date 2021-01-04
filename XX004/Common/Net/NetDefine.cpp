/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetDefine.cpp
* Summary: 网络相关定义定义实现。
*
* Author: XuXiang
* Date: 2021-01-04 23:20
*******************************************************/

#include "NetDefine.h"

namespace XX004
{
	namespace Net
	{
		bool socket_set_nonblocking(socket_t s, bool on)
		{
#if defined(WIN)
			u_long argp = on ? 1 : 0;
			return ::ioctlsocket(s, FIONBIO, &argp) == 0;
#else
			int32_t opts = fcntl(s, F_GETFL, 0);
			if (opts < 0)
			{
				return false;
			}

			opts = on ? (opts | O_NONBLOCK) : (opts & ~O_NONBLOCK);
			fcntl(s, F_SETFL, opts);
			return opts >= 0;
#endif
		}
	}
}
