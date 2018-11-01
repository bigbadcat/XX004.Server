﻿/*******************************************************
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

#include "Macro.h"
#include <thread>
#include <string>
#include "Core\semaphore.h"
using namespace std;

namespace XX004
{
	class NetManager;
	class ServerBase;

	//创建ServerXXX和NetManagerXXXX
	//创建线程建立游戏循环
	//提供程序统一流程 Start Loop Stop 
	//维护网络消息和命令消息队列
	class ServerMain
	{
	public:
		ServerMain();
		virtual ~ServerMain();

		int Run();

		//获取网络管理
		inline NetManager* GetNetManager()const { return m_pNetManager; }

		//获取服务对象
		inline ServerBase* GetServe()const { return m_pServer; }

		//获取当前Main对象
		static inline ServerMain* GetCurMain() { return pCurMain; }

	protected:
		//创建网络管理
		virtual NetManager* OnCreateNetManager() = 0;

		//创建服务模块
		virtual ServerBase* OnCreateServer() = 0;

	private:
		//线程过程
		void ThreadProcess();

		//服务器循环
		void SeverLoop();

		//启动分步
		bool StarStep(int step);

		//更新分步
		bool UpdateStep(UInt64 &sectick);

		//停止分步
		bool StopStep(int step);

		//当前Main对象
		static ServerMain* pCurMain;

		//每一帧的间隔毫秒
		static const Int64 FRAME_GAP;

		//退出命令
		static const string COMMAND_QUIT;

		//网络管理
		NetManager* m_pNetManager;

		//服务对象
		ServerBase* m_pServer;

		//逻辑处理线程
		thread m_Thread;

		//是否运行中
		bool m_IsRunning;

		//启动信号
		xxstd::semaphore m_StartSemaphore;
	};
}

#endif	//__ServerMain_h__
