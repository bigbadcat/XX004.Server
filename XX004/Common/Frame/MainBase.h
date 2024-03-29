﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainBase.h
* Summary: 程序入口管理的基类。
*
* Author: XuXiang
* Date: 2018-08-13 22:43
*******************************************************/

#ifndef __MainBase_h__
#define __MainBase_h__

#include "../Macro.h"
#include <string>
using namespace std;

namespace XX004
{
	class StorageManager;
	class NetManagerBase;
	class ServerBase;

	//创建ServerXXX和NetManagerXXXX
	class MainBase
	{
	public:
		MainBase();
		virtual ~MainBase();

		//解析服务器ID
		static int ParseServerID(int argc, char *argv[]);

		//启动
		int Run(int type, int sid);

		//获取服务类型
		inline int GetType()const { return m_Type; }

		//获取网络管理
		inline NetManagerBase* GetNetManager()const { return m_pNetManager; }

		//获取存储管理
		inline StorageManager* GetStorageManager()const { return m_pStorageManager; }

		//获取服务对象
		inline ServerBase* GetServer()const { return m_pServer; }

		//获取当前Main对象
		static inline MainBase* GetCurMain() { return pCurMain; }

	protected:
		//创建网络管理
		virtual NetManagerBase* OnCreateNetManager() = 0;

		//创建服务模块
		virtual ServerBase* OnCreateServer() = 0;

	private:

		//当前Main对象
		static MainBase *pCurMain;

		//每一帧的间隔毫秒
		static const Int64 FRAME_GAP;

		//退出命令
		static const string COMMAND_QUIT;

		//服务器类型
		int m_Type;

		//网络管理
		NetManagerBase *m_pNetManager;

		//存储管理
		StorageManager *m_pStorageManager;

		//服务对象
		ServerBase *m_pServer;
	};
}

#endif
