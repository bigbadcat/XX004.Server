/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManager.cpp
* Summary: 网络管理的基类。
*
* Author: XuXiang
* Date: 2018-10-24 20:52
*******************************************************/

#include "NetManager.h"
#include <iostream>
#include <string>
using namespace std;

namespace XX004
{
	NetManager::NetManager()
	{
	}

	NetManager::~NetManager()
	{
	}

	void NetManager::OnConnected(NetConnection *connection)
	{

	}

	void NetManager::OnDisconnected(NetConnection *connection)
	{

	}

	void NetManager::OnRecvData(NetConnection *connection, Int32 cmd, Byte *buffer, int len)
	{
		//解析网络消息并添加到队列中
	}

	void NetManager::RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call)
	{
		m_CallBack[cmd] = call;
	}

	void NetManager::UnregisterMessageCallBack(Int32 cmd)
	{
		m_CallBack.erase(cmd);
	}

	void NetManager::UnregisterAllCallBack()
	{
		m_CallBack.clear();
	}

	void NetManager::OnUpdate()
	{
		//分发消息队列
	}

	void NetManager::Test(Int32 cmd)
	{
		MessageCallBackMap::iterator itr = m_CallBack.find(cmd);
		if (itr != m_CallBack.end())
		{
			(itr->second)(cmd, NULL, 0);
		}
	}
}