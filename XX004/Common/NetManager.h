/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManager.h
* Summary: 网络管理的基类。
*
* Author: XuXiang
* Date: 2018-10-24 20:52
*******************************************************/

#ifndef __NetManager_h__
#define __NetManager_h__

#include <map>
#include <functional>
#include "Net/NetServer.h"
using namespace XX004::Net;

namespace XX004
{
	//网络消息回调
	typedef std::function<void(Int32, Byte*, int)> NetMessageCallBack;
	typedef std::map<Int32, NetMessageCallBack> MessageCallBackMap;

	//网络连接对象
	class NetManager : public INetProcesser
	{
	public:
		NetManager();
		virtual ~NetManager();

		//启动网络 ipaddress:IP地址。 port:端口号。
		void Start(const string &ipaddress, int port);

		//停止网络
		void Stop();

		virtual void OnConnected(NetConnection *connection);
		virtual void OnDisconnected(NetConnection *connection);
		virtual void OnRecvData(NetConnection *connection, const NetPackageHeader& header, Byte *buffer);

		//注册网络消息处理函数
		void RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call);

		//注销网络消息处理函数
		void UnregisterMessageCallBack(Int32 cmd);

		//注销所有网络消息处理函数
		void UnregisterAllCallBack();

		//帧更新
		virtual void OnUpdate();

		void Test(Int32 cmd);

	private:
		//消息集合
		MessageCallBackMap m_CallBack;

		//网络服务端模块
		NetServer m_Server;
	};
}

#endif	//__NetManager_h__