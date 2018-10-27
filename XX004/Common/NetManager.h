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

		virtual void OnConnected(NetConnection *connection);
		virtual void OnDisconnected(NetConnection *connection);
		virtual void OnRecvData(NetConnection *connection, Int32 cmd, Byte *buffer, int len);

		//注册网络消息处理函数
		void RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call);

		//注销网络消息处理函数
		void UnregisterMessageCallBack(Int32 cmd);

		//注销所有网络消息处理函数
		void UnregisterAllCallBack();

		void Test(Int32 cmd);

	private:
		//消息集合
		MessageCallBackMap m_CallBack;
	};
}

#endif	//__NetManager_h__