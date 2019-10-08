/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: LoginModule.h
* Summary: 登陆模块。
*
* Author: XuXiang
* Date: 2019-10-03 11:27
*******************************************************/

#ifndef __LoginModule_h__
#define __LoginModule_h__

#include "../ModuleBase.h"
#include "UserInfo.h"
#include <map>
using namespace std;

namespace XX004
{
	typedef std::map<Int64, std::string> UIDToUserMap;

	//登陆模块
	class LoginModule : public ModuleBase
	{
	public:
		//角色ID序号占位
		static const int ROLE_ID_STAMP_BIT;

		//构造析构函数
		LoginModule();
		virtual ~LoginModule();

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

		//通过用户名获取链接标识。
		Int64 GetUID(const string& username);

		//通过用户名获取用户信息
		UserInfo* GetUser(const string& username);

		//通过链接标识获取用户名
		UserInfo* GetUser(Int64 uid);

		void OnConnect(NetDataItem *item);
		void OnDisconnect(NetDataItem *item);

	private:

		//添加用户信息
		void AddUserInfo(const string& username, UInt64 uid);

		//用户离线
		void OnUserOutline(const string& username);

		//用户退出游戏，调用后username对应的UserInfo将无效
		void OnUserQuit(const string& username);

		
		void OnLoginRequest(NetDataItem *item);					//登陆请求		
		void OnCreateRoleRequest(NetDataItem *item);			//角色创建请求
		void OnEnterGameRequest(NetDataItem *item);				//进入游戏请求
		void OnQuitGameRequest(NetDataItem *item);				//退出游戏请求
		
		static void OnRoleListRequest(NetDataItem *item);		//请求用户角色数据		
		void OnRoleListResponse(NetDataItem *item);				//回复用户角色数据
		static void OnRoleAddRequest(NetDataItem *item);		//请求添加角色
		void OnRoleAddResponse(NetDataItem *item);				//回复添加角色


		//用户信息
		UserInfoMap m_Users;

		//连接uid到用户名的映射
		UIDToUserMap m_UIDToUserName;
	};
}

#endif