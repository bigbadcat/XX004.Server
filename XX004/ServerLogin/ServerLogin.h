﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerLogin.h
* Summary: 登陆服务模块。
*
* Author: XuXiang
* Date: 2018-12-18 19:56
*******************************************************/

#ifndef __ServerLogin_h__
#define __ServerLogin_h__

#include "ServerBase.h"
#include "UserInfo.h"

namespace XX004
{
	class ServerLogin : public ServerBase
	{
	public:
		ServerLogin();
		virtual ~ServerLogin();

		//最大角色数量
		static const int MAX_ROLE_NUMBER;

		//检测名称是否合法
		static bool CheckName(const std::string& name);

	protected:
		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr);

		//初始化配置列表
		virtual void OnAddConfig(vector<ModuleConfig*> &cfgs);

		//启动服务
		virtual bool OnInitStep(int step, float &r);

		//停止服务
		virtual bool OnReleaseStep(int step, float &r);

		//帧更新
		virtual void OnUpdate();

		//秒更新
		virtual void OnUpdatePerSecond();

		//收到命令
		virtual void OnCommand(const std::string& cmd, const std::vector<std::string> &param);

	private:
		void OnConnect(NetDataItem *item);
		void OnDisconnect(NetDataItem *item);

		void OnLoginRequest(NetDataItem *item);
		void OnUserInfoResponse(NetDataItem *item);
		void OnCreateRoleRequest(NetDataItem *item);
		void OnRoleStampResponse(NetDataItem *item);
		void OnRoleAddResponse(NetDataItem *item);

		//登陆的角色
		UserInfoMap m_UserInfos;
	};
}

#endif