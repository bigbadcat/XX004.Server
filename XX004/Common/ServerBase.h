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

#include <iostream>
#include <string>
#include <vector>
#include "NetManagerBase.h"
#include "StorageManager.h"
#include "Core/semaphore.h"
#include "Util/LockQueue.h"
#include "Config/ModuleConfig.h"
using namespace std;

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

	//命令信息
	struct CommandInfo
	{
		string cmd;							//命令
		vector<string> params;				//参数

		//构造函数
		CommandInfo() {}
		CommandInfo(const string& line) { Init(line); }
		
		//初始化
		bool Init(const string& line);

		//判断命令是否有效
		inline bool IsValid()const { return cmd.size() > 0; }
	};

	std::ostream & operator<<(std::ostream &out, const CommandInfo &info);

	//服务模块的基类
	class ServerBase
	{
	public:
		ServerBase();
		virtual ~ServerBase();

		//获取当前服务状态
		inline ServerState GetState()const { return m_State; }

		//启动服务
		//id:服务器编号
		//wait_init_finish:是否等待初始化完成才返回
		void Start(int id, bool wait_init_finish);

		//停止服务
		void Stop();

		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr) = 0;

		//注册存储消息
		virtual void RegisterStorageMessage(StorageManager *pMgr) = 0;

		//提交命令
		void PostCommand(const std::string& cmd_line);

		//获取服务器组
		int GetServerGroup()const { return m_Group; }

		//获取服务器编号
		int GetServerID()const { return m_ID; }

		//每一帧的间隔毫秒
		static const Int64 FRAME_GAP;

	protected:

		//初始化配置列表
		virtual void OnAddConfig(vector<ModuleConfig*> &cfgs);

		//初始化过程
		//step:初始化分步计数
		//r:初始化进度0-1
		//ret:是否初始化完成
		virtual bool OnInitStep(int step) = 0;

		//释放过程
		//step:释放分步计数
		//r:释放进度0-1
		//ret:是否释放完成
		virtual bool OnReleaseStep(int step) = 0;

		//帧更新
		virtual void OnUpdate() = 0;

		//秒更新
		virtual void OnUpdatePerSecond() = 0;

		//收到命令
		virtual void OnCommand(const std::string& cmd, const std::vector<std::string> &param);

	private:
		//线程过程
		void ThreadProcess();

		//命令处理
		void HandleCommand();

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

		//命令队列
		LockQueue<CommandInfo> m_Cmds;

		//配置列表
		vector<ModuleConfig*> m_Configs;

		//服务器组
		int m_Group;

		//服务器编号
		int m_ID;
	};
}

#endif
