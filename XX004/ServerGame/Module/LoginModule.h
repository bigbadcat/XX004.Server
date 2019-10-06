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

#include "ModuleBase.h"

namespace XX004
{
	class LoginModule : public ModuleBase
	{
	public:
		LoginModule();
		virtual ~LoginModule();

		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr);

		//注册存储消息
		virtual void RegisterStorageMessage(StorageManager *pMgr);

		//初始化 mysql:数据库访问接口，不要保存起来。
		virtual void Init();

		//销毁
		virtual void Release();

	protected:

	private:

		//登陆请求
		void OnLoginRequest(NetDataItem *item);

		//用户角色数据请求
		void OnRoleListRequest(NetDataItem *item);

		//用户数据回复
		void OnRoleListResponse(NetDataItem *item);

		//角色创建下一序号
		int m_NextRoleStamp;
	};
}

#endif