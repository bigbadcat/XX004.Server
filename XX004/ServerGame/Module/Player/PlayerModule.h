/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerModule.h
* Summary: 玩家模块。
*
* Author: XuXiang
* Date: 2019-10-21 20:41
*******************************************************/

#ifndef __PlayerModule_h__
#define __PlayerModule_h__

#include <Module/ModuleBase.h>
#include <map>
using namespace std;

namespace XX004
{
	class PlayerBasicData;
	typedef map<Int64, PlayerBasicData*> PlayerMap;

	//登陆模块
	class PlayerModule : public ModuleBase
	{
	public:
		//角色ID序号占位
		static const int ROLE_ID_STAMP_BIT;

		//构造析构函数
		PlayerModule();
		virtual ~PlayerModule();

		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr);

		//注册存储消息
		virtual void RegisterStorageMessage(StorageManager *pMgr);

		//初始化配置列表
		virtual void AddConfig(vector<ModuleConfig*> &cfgs);

		//初始化
		virtual void Init();

		//销毁
		virtual void Release();

	private:

		//void OnLoginRequest(NetDataItem *item);					//登陆请求

		//static void OnRoleListRequest(NetDataItem *item);		//请求用户角色数据		
		//void OnRoleListResponse(NetDataItem *item);				//回复用户角色数据

		//当前在线的玩家
		PlayerMap m_Players;
	};
}

#endif