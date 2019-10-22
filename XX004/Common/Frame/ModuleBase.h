/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ModuleBase.h
* Summary: 模块基类。
*
* Author: XuXiang
* Date: 2019-10-03 11:37
*******************************************************/

#ifndef __ModuleBase_h__
#define __ModuleBase_h__

#include "../Macro.h"
#include "../Config/ModuleConfig.h"

namespace XX004
{
	class ServerBase;
	class NetManagerBase;	
	class StorageManager;
	class MySQLWrap;
	struct NetDataItem;

	namespace Net
	{
		class NetMessage;
	}

	class ModuleBase
	{
	public:
		ModuleBase();
		virtual ~ModuleBase();

		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr) = 0;

		//注册存储消息
		virtual void RegisterStorageMessage(StorageManager *pMgr) = 0;

		//初始化配置列表
		virtual void AddConfig(vector<ModuleConfig*> &cfgs) = 0;

		//初始化
		virtual void Init();

		//销毁
		virtual void Release();

		//获取服务
		static ServerBase* GetServer();

		//获取数据库
		static MySQLWrap* GetMySQL();

		//发送网络消息 rid:角色编号
		static void SendNet(Int64 rid, int command, Net::NetMessage *msg);

		//存储请求
		static void RequestStorage(Int64 rid, int command, Net::NetMessage *msg);

		//存储回复
		static void ResponseStorage(Int64 rid, int command, Net::NetMessage *msg);

	};
}

#endif