/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerGate.h
* Summary: 网关服务模块。
*
* Author: XuXiang
* Date: 2018-12-03 19:42
*******************************************************/

#ifndef __ServerGate_h__
#define __ServerGate_h__

#include "ServerBase.h"
#include <map>
#include <string>

namespace XX004
{
	class ServerGate : public ServerBase
	{
		typedef std::map<Int64, std::string> UIDToUserNameMap;
		typedef std::map<std::string, Int64> UserNameToUIDMap;

	public:
		ServerGate();
		virtual ~ServerGate();

		//设置关联 uid:连接id username:用户名称
		void SetRelation(Int64 uid, const std::string& username);

		//移除关联
		void RemoveRelation(Int64 uid);

	protected:
		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr);

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

		//连接uid到用户名的映射
		UIDToUserNameMap m_UIDToUserName;

		//用户名到连接uid的映射(与m_UIDToUserName保持一致)
		UserNameToUIDMap m_UserNameToUID;
	};
}

#endif	//__ServerGate_h__