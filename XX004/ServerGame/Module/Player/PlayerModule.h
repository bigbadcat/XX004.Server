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

#include <Frame/ModuleBase.h>
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

		//获取角色
		PlayerBasicData* GetPlayer(Int64 rid);

	private:

		void OnRenameRequest(NetDataItem *item);					//改名请求

		static void OnDBBasicInfoRequest(NetDataItem *item);		//请求角色基本信息
		void OnDBBasicInfoResponse(NetDataItem *item);				//回复角色基本信息
		static void OnDBBasicSaveRequest(NetDataItem *item);		//请求保存角色基本信息
		static void OnDBRenameRequest(NetDataItem *item);			//请求角色改名
		void OnDBRenameResponse(NetDataItem *item);					//回复角色改名

		EVENT_DEFINE(PlayerModule, UserOnline);						//玩家上线
		EVENT_DEFINE(PlayerModule, UserOutline);					//玩家掉线

		//当前在线的玩家
		PlayerMap m_Players;
	};
}

#endif