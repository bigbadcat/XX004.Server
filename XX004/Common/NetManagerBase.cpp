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
#include <assert.h>
#include <string>
using namespace std;

namespace XX004
{
	NetDataItem::NetDataItem()
	{
		Reset();
	}

	void NetDataItem::Reset()
	{
		sid = 0;
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
		NetDataItem *item = GetNetDataItem();
		item->sid = (Int64)connection->GetSocket();
		item->cmd = header.Command;
		item->len = header.BodySize;
		::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, item->len);
		{
			std::unique_lock<std::mutex> lock(m_RecvMutex);
			m_RecvQueue.push(item);
		}
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

	void NetManagerBase::Start()
	{
		cout << "NetManagerBase::Start" << endl;
		JoinThread(m_Thread);
		m_IsRunning = true;
		m_Thread = thread([](NetManagerBase *t){t->ThreadProcess(); }, this);
		m_InitSemaphore.wait();
	}

	void NetManagerBase::Stop()
	{
		cout << "NetManagerBase::Stop" << endl;
		m_IsRunning = false;
	}

	void NetManagerBase::Join()
	{
		JoinThread(m_Thread);
	}

	void NetManagerBase::Dispatch()
	{
		//先进行非线程安全的只读判断
		if (m_RecvQueue.size() <= 0)
		{
			return;
		}

		//先锁定队列，将消息都拿出来，解锁队列
		static NetDataItemQueue temp_queue;
		{
			std::unique_lock<std::mutex> lock(m_RecvMutex);
			while (m_RecvQueue.size() > 0)
			{
				NetDataItem *item = m_RecvQueue.front();
				m_RecvQueue.pop();
				temp_queue.push(item);
			}
		}

		//分发消息
		while (temp_queue.size() > 0)
		{
			NetDataItem *item = temp_queue.front();
			temp_queue.pop();
			MessageCallBackMap::iterator itr = m_CallBack.find(item->cmd);
			if (itr != m_CallBack.end())
			{
				(itr->second)(item);
			}
			CacheNetDataItem(item);
		}
	}

	void NetManagerBase::Send(Int64 sid, int command, Byte *buffer, int len)
	{
		NetDataItem *item = GetNetDataItem();
		item->sid = sid;
		item->cmd = command;
		::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, len);
		item->len = len;
		{
			std::unique_lock<std::mutex> lock(m_SendMutex);
			m_SendQueue.push(item);
		}
	}

	NetDataItem* NetManagerBase::GetNetDataItem()
	{
		//先从缓存队列中获取
		NetDataItem *item = NULL;
		{
			std::unique_lock<std::mutex> lock(m_CacheMutex);
			if (m_CacheQueue.size() > 0)
			{
				item = m_CacheQueue.front();
				m_CacheQueue.pop();
			}
		}
		
		//没有再创建
		if (item == NULL)
		{
			item = new NetDataItem();
		}
		item->Reset();
		return item;
	}

	void NetManagerBase::CacheNetDataItem(NetDataItem *item)
	{
		assert(item != NULL);

		std::unique_lock<std::mutex> lock(m_CacheMutex);
		m_CacheQueue.push(item);
	}

	void NetManagerBase::ThreadProcess()
	{
		m_Server.SetProcesser(this);
		m_Server.Start("127.0.0.1", 9000);
		m_InitSemaphore.post();

		std::chrono::milliseconds dura(100);
		while (m_IsRunning)
		{
			this->OnPostSend();
			this->OnSocketSelect();
			std::this_thread::sleep_for(dura);
		}
		m_Server.Stop();
	}

	void NetManagerBase::OnPostSend()
	{
		//先进行非线程安全的只读判断
		if (m_SendQueue.size() <= 0)
		{
			return;
		}

		//先锁定队列，将消息都拿出来，解锁队列
		static NetDataItemQueue temp_queue;
		{
			std::unique_lock<std::mutex> lock(m_SendMutex);
			while (m_SendQueue.size() > 0)
			{
				NetDataItem *item = m_SendQueue.front();
				m_SendQueue.pop();
				temp_queue.push(item);
			}
		}

		//提交发送数据
		while (temp_queue.size() > 0)
		{
			NetDataItem *item = temp_queue.front();
			temp_queue.pop();
			OnPost(item);
			CacheNetDataItem(item);
		}
	}

	void NetManagerBase::OnSocketSelect()
	{
		m_Server.SelectSocket();
	}

	void NetManagerBase::OnPost(NetDataItem *item)
	{
		NetConnection *pcon = m_Server.GetGetConnection((SOCKET)item->sid);
		if (pcon != NULL)
		{
			NetPackageHeader sendhead;
			sendhead.SetSign();
			sendhead.Command = item->cmd;
			sendhead.BodySize = item->len;

			static Byte sendbuff[1024];
			int sendsize = 0;
			sendsize = sendhead.Pack(sendbuff, sendsize);
			::memcpy_s(sendbuff + sendsize, 1024 - sendsize, item->data, item->len);
			sendsize += item->len;

			bool cansend = pcon->AddSendData(sendbuff, sendsize);
			if (!cansend)
			{
				//同一网络周期内发送数据量过大或者该连接速度太低把数据缓冲区挤爆了，要断掉连接
				m_Server.CloseConnection(pcon);
				pcon = NULL;
			}
		}
	}
}