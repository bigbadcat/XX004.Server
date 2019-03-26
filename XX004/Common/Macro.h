/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: Macro.h
* Summary: 通用宏定义。
*
* Author: XuXiang
* Date: 2018-08-26 22:02
*******************************************************/

#ifndef __Macro_h__
#define __Macro_h__

#include <thread>
#include <string>
#include <iostream>

namespace XX004
{
	typedef unsigned char Byte;
	typedef char SByte;
	typedef short Int16;
	typedef unsigned short UInt16;
	typedef int Int32;
	typedef unsigned int UInt32;
	typedef __int64 Int64;
	typedef unsigned __int64 UInt64;

#define SAFE_DELETE(p) if (p!=NULL){delete(p); p=NULL;}
#define SAFE_DELETE_ARRAY(p) if (p!=NULL){delete[](p); p=NULL;}
#define SAFE_DELETE_MAP(p) for (auto itr = p.begin(); itr != p.end(); ++itr){ SAFE_DELETE(itr->second); } p.clear();

	//等待线程结束
	inline void JoinThread(std::thread &t)
	{
		if (t.joinable())
		{
			t.join();
		}
	}

	//将光标往回移动
	inline void MoveCursorBack(int n)
	{
		while (n-- > 0)
		{
			printf("\b");
		}
	}
}

#endif	//__Macro_h__
