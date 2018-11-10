/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerBase.cpp
* Summary: 网络管理的基类。
*
* Author: XuXiang
* Date: 2018-10-24 20:52
*******************************************************/

#include "NetManagerBase.h"
#include "Util/DataUtil.h"
#include <iostream>
#include <string>
using namespace std;

namespace XX004
{
	void NetDataItem::Reset()
	{
		s = SOCKET_ERROR;
		key = RemoteKey(RemoteType::RT_UNKNOW, 0);
		cmd = 0;
		len = 0;
	}

	NetManagerBase::NetManagerBase()
	{
	}

	NetManagerBase::~NetManagerBase()
	{
		while (m_CacheQueue.size() > 0)
		{
			NetDataItem *item = m_CacheQueue.front();
			m_CacheQueue.pop();
			SAFE_DELETE(item);
		}
		while (m_SendQueue.size() > 0)
		{
			NetDataItem *item = m_SendQueue.front();
			m_SendQueue.pop();
			SAFE_DELETE(item);
		}
		while (m_RecvQueue.size() > 0)
		{
			NetDataItem *item = m_RecvQueue.front();
			m_RecvQueue.pop();
			SAFE_DELETE(item);
		}
	}

	void NetManagerBase::Start(const string &ipaddress, int port)
	{
		m_Server.SetProcesser(this);
		m_Server.Start(ipaddress, port);
	}

	void NetManagerBase::Stop()
	{
		m_Server.Stop();
	}

	void NetManagerBase::OnConnected(NetConnection *connection)
	{
		//cout << "NetManagerBase::OnConnected ip:" << connection->GetIPAddress() << " port:" << connection->GetPort() << endl;
	}

	void NetManagerBase::OnDisconnected(NetConnection *connection)
	{
		//cout << "NetManagerBase::OnDisconnected ip:" << connection->GetIPAddress() << " port:" << connection->GetPort() << endl;
	}

	void NetManagerBase::OnRecvData(NetConnection *connection, const NetPackageHeader& header, Byte *buffer)
	{
		//解析网络消息并添加到队列中
		cout << "NetManagerBase::OnRecvData ip:" << connection->GetIPAddress() << " port:" << connection->GetPort();
		cout << " cmd:" << header.Command << " len:" << header.BodySize << endl;

		//包头相关处理

		//加入消息队列
		NetDataItem *item = CreateNetDataItem();
		item->s = connection->GetSocket();
		item->key = connection->GetRemote();
		item->cmd = header.Command;
		item->len = header.BodySize;
		::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, item->len);
		{
			std::unique_lock<std::mutex> lock(m_RecvMutex);
			m_RecvQueue.push(item);
		}

		////分发消息
		//MessageCallBackMap::iterator itr = m_CallBack.find(header.Command);
		//if (itr != m_CallBack.end())
		//{
		//	(itr->second)(connection, header.Command, buffer);
		//}
	}

	void NetManagerBase::RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call)
	{
		m_CallBack[cmd] = call;
	}

	void NetManagerBase::UnregisterMessageCallBack(Int32 cmd)
	{
		m_CallBack.erase(cmd);
	}

	void NetManagerBase::UnregisterAllCallBack()
	{
		m_CallBack.clear();
	}

	void NetManagerBase::OnUpdate()
	{
		//分发接收消息队列
		if (m_RecvQueue.size() > 0)
		{
			std::unique_lock<std::mutex> lock(m_RecvMutex);
			if (m_RecvQueue.size() > 0)
			{
				NetDataItem *item = m_RecvQueue.front();
				m_RecvQueue.pop();

				MessageCallBackMap::iterator itr = m_CallBack.find(item->cmd);
				if (itr != m_CallBack.end())
				{
					(itr->second)(item);
				}
				m_CacheQueue.push(item);
			}
			
		}

		//写入发送消息队列
	}

	NetDataItem* NetManagerBase::CreateNetDataItem()
	{
		NetDataItem *item = NULL;
		if (m_CacheQueue.size() > 0)
		{
			item = m_CacheQueue.front();			
			m_CacheQueue.pop();
		}
		else
		{
			item = new NetDataItem();
		}
		item->Reset();
		return item;
	}

	//void NetManagerBase::Test(Int32 cmd)
	//{
	//	MessageCallBackMap::iterator itr = m_CallBack.find(cmd);
	//	if (itr != m_CallBack.end())
	//	{
	//		(itr->second)(cmd, NULL);
	//	}
	//}
}