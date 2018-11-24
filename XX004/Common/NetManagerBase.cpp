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
		op = NetOperateType::NOT_UNKNOW;
		uid = 0;
		key = RemoteKey(RemoteType::RT_UNKNOW, 0);
		cmd = 0;
		len = 0;
	}

	NetManagerBase::NetManagerBase()
	{
	}

	NetManagerBase::~NetManagerBase()
	{
	}

	void NetManagerBase::OnConnected(NetConnection *connection)
	{
		//cout << "NetManagerBase::OnConnected ip:" << connection->GetIPAddress() << " port:" << connection->GetPort() << endl;
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_CONNECT;
		item->uid = connection->GetUniqueID();
		item->key = connection->GetRemote();
		m_RecvQueue.Push(item);
	}

	void NetManagerBase::OnDisconnected(NetConnection *connection)
	{
		//cout << "NetManagerBase::OnDisconnected ip:" << connection->GetIPAddress() << " port:" << connection->GetPort() << endl;
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DISCONNECT;
		item->uid = connection->GetUniqueID();
		item->key = connection->GetRemote();
		m_RecvQueue.Push(item);
	}

	void NetManagerBase::OnRecvData(NetConnection *connection, const NetPackageHeader& header, Byte *buffer)
	{
		//解析网络消息并添加到队列中
		cout << "NetManagerBase::OnRecvData ip:" << connection->GetIPAddress() << " port:" << connection->GetPort();
		cout << " cmd:" << header.Command << " len:" << header.BodySize << endl;

		//内置协议

		//包头相关处理

		//加入消息队列
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DATA;
		item->uid = connection->GetUniqueID();
		item->key = connection->GetRemote();
		item->cmd = header.Command;
		item->len = header.BodySize;
		::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, item->len);
		OnAddRecvData(item);		
	}

	void NetManagerBase::RegisterMessageCallBack(Int32 cmd, NetMessageCallBack call)
	{
		m_CallBack[cmd] = call;
	}

	NetMessageCallBack NetManagerBase::GetMessageCallBack(Int32 cmd)const
	{
		MessageCallBackMap::const_iterator citr = m_CallBack.find(cmd);
		return citr == m_CallBack.cend() ? NULL : citr->second;
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
		if (m_RecvQueue.GetSize() <= 0)
		{
			return;
		}

		//先锁定队列，将消息都拿出来，解锁队列
		static NetDataItemQueue temp_queue;
		m_RecvQueue.MoveAll(temp_queue);

		//分发消息
		while (temp_queue.size() > 0)
		{
			NetDataItem *item = temp_queue.front();
			temp_queue.pop();
			OnDispatch(item);
			CacheNetDataItem(item);
		}
	}

	void NetManagerBase::Update(UInt64 uid, const RemoteKey& key)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_UPDATE;
		item->uid = uid;
		item->key = key;
		m_SendQueue.Push(item);
	}

	void NetManagerBase::Send(UInt64 uid, int command, Byte *buffer, int len)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DATA;
		item->uid = uid;
		item->cmd = command;
		::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, len);
		item->len = len;
		m_SendQueue.Push(item);
	}

	void NetManagerBase::Send(const RemoteKey& key, int command, Byte *buffer, int len)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DATA;
		item->key = key;
		item->cmd = command;
		::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, len);
		item->len = len;
		m_SendQueue.Push(item);
	}

	void NetManagerBase::Close(UInt64 uid)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_CLOSE;
		item->uid = uid;
		m_SendQueue.Push(item);
	}

	void NetManagerBase::Close(const RemoteKey& key)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_CLOSE;
		item->key = key;
		m_SendQueue.Push(item);
	}

	void NetManagerBase::OnAddRecvData(NetDataItem *item)
	{
		//默认放入分发队列，网关会重写此函数，对网络数据进行转发
		m_RecvQueue.Push(item);
	}

	NetDataItem* NetManagerBase::GetNetDataItem()
	{
		//先从缓存队列中获取
		NetDataItem *item = m_CacheQueue.TryPop();
		if (item == NULL)
		{
			//没有再创建
			item = new NetDataItem();
		}
		item->Reset();
		return item;
	}

	void NetManagerBase::CacheNetDataItem(NetDataItem *item)
	{
		assert(item != NULL);
		m_CacheQueue.Push(item);
	}

	void NetManagerBase::OnDispatch(NetDataItem *item)
	{
		if (item->op == NetOperateType::NOT_CONNECT)
		{
			if (m_OnConnectCallBack != NULL)
			{
				m_OnConnectCallBack(item);
			}
		}
		else if (item->op == NetOperateType::NOT_DISCONNECT)
		{
			if (m_OnDisconnectCallBack != NULL)
			{
				m_OnDisconnectCallBack(item);
			}
		}
		else if (item->op == NetOperateType::NOT_DATA)
		{
			MessageCallBackMap::iterator itr = m_CallBack.find(item->cmd);
			if (itr != m_CallBack.end())
			{
				(itr->second)(item);
			}
		}
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
		if (m_SendQueue.GetSize() <= 0)
		{
			return;
		}

		//先锁定队列，将消息都拿出来，解锁队列
		static NetDataItemQueue temp_queue;
		m_SendQueue.MoveAll(temp_queue);

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
		if (item->op == NetOperateType::NOT_DATA)
		{
			OnPostData(item);
		}
		else if (item->op == NetOperateType::NOT_UPDATE)
		{
			m_Server.SetRemote(item->uid, item->key);
		}
		else if (item->op == NetOperateType::NOT_CLOSE)
		{
			NetConnection *pcon = item->uid != 0 ? m_Server.GetConnection(item->uid) : m_Server.GetConnection(item->key);
			if (pcon != NULL)
			{
				m_Server.CloseConnection(pcon);
			}
		}
	}

	void NetManagerBase::OnPostData(NetDataItem *item)
	{
		NetConnection *pcon = item->uid != 0 ? m_Server.GetConnection(item->uid) : m_Server.GetConnection(item->key);
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