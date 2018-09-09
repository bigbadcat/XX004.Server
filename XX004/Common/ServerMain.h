/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerMain.h
* Summary: 程序入口管理的基类。
*
* Author: XuXiang
* Date: 2018-08-13 22:43
*******************************************************/

#ifndef __ServerMain_h__
#define __ServerMain_h__

namespace XX004
{
	//创建ServerXXX和NetManagerXXXX
	//创建线程建立游戏循环
	//提供程序统一流程 Start Loop Stop 
	//维护网络消息和命令消息队列
	class ServerMain
	{
	public:
		ServerMain();

		int Run();

	private:


	};
}

#endif	//__ServerMain_h__
