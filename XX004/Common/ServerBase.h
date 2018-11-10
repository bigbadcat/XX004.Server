/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerBase.h
* Summary: 服务模块的基类。
*
* Author: XuXiang
* Date: 2018-10-25 19:24
*******************************************************/

#ifndef __ServerBase_h__
#define __ServerBase_h__

#include "NetManagerBase.h"
#include <thread>
#include <string>
#include "Core\semaphore.h"

namespace XX004
{
	enum ServerState
	{
		SS_CREATE = 0,
		SS_INIT = 1,
		SS_UPDATE = 2,
		SS_RELEASE = 3,
		SS_END = 4,
	};

	//服务模块的基类
	class ServerBase
	{
	public:
		ServerBase();
		virtual ~ServerBase();

		//获取当前服务状态
		inline ServerState GetState()const { return m_State; }

		//启动服务
		//wait_init_finish 是否等待初始化完成才返回
		void Start(bool wait_init_finish);

		//停止服务
		void Stop();

		//等待结束
		void Join();

		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr) = 0;

		//每一帧的间隔毫秒
		static const Int64 FRAME_GAP;

	protected:

		//初始化过程
		//step:初始化分步计数
		//r:初始化进度0-1
		//ret:是否初始化完成
		virtual bool OnInitStep(int step, float &r) = 0;

		//释放过程
		//step:释放分步计数
		//r:释放进度0-1
		//ret:是否释放完成
		virtual bool OnReleaseStep(int step, float &r) = 0;

		//帧更新
		virtual void OnUpdate() = 0;

		//秒更新
		virtual void OnUpdatePerSecond() = 0;

	private:
		//线程过程
		void ThreadProcess();

		//初始化
		void Init();

		//更新
		void Update();

		//释放
		void Release();

		//服务状态
		ServerState m_State;

		//逻辑处理线程
		thread m_Thread;

		//启动信号
		xxstd::semaphore m_InitSemaphore;
	};
}

#endif	//__ServerBase_h__
