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
#include "../Util/DataUtil.h"
#include "StartSetting.h"
#include "../Protocol/NetMsgID.h"
#include "MainBase.h"
#include <iostream>
#include <assert.h>
#include <string>
using namespace std;

namespace XX004
{
#define INTERNAL_CALL(id, fun) make_pair(id, [this](NetConnection *con, Int32 cmd, Byte *buffer, int len){this->fun(con, cmd, buffer, len); })

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

	std::ostream & operator<<(std::ostream &out, RemoteKey &key)
	{
		out << "[" << key.first << "," << key.second << "]";
		return out;
	}

	NetManagerBase::NetManagerBase() : m_Port(0)
	{
		m_InternalCallBack.insert(INTERNAL_CALL(NetMsgID::REMOTE_IDENTIFY, OnMsgRemoteIdentify));
	}

	NetManagerBase::~NetManagerBase()
	{
		SAFE_DELETE_MAP(m_InternalConnections);
	}

	void NetManagerBase::OnConnected(NetConnection *connection)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_CONNECT;
		item->uid = connection->GetUniqueID();
		item->key = connection->GetRemote();
		m_RecvQueue.Push(item);
	}

	void NetManagerBase::OnDisconnected(NetConnection *connection)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DISCONNECT;
		item->uid = connection->GetUniqueID();
		item->key = connection->GetRemote();
		m_RecvQueue.Push(item);
	}

	void NetManagerBase::OnRecvData(NetConnection *connection, const NetPackageHeader& header, Byte *buffer)
	{
		//包头相关处理
		
		//数据处理
		NetMessageInternalCallBackMap::iterator itr = m_InternalCallBack.find(header.Command);
		if (itr != m_InternalCallBack.end())
		{
			//内部处理
			itr->second(connection, header.Command, buffer, header.BodySize);			
		}
		else
		{
			//加入消息队列
			NetDataItem *item = GetNetDataItem();
			item->op = NetOperateType::NOT_DATA;
			item->uid = connection->GetUniqueID();
			item->key.first = connection->GetRomoteType();
			item->key.second = header.GUID;
			item->cmd = header.Command;
			item->len = header.BodySize;
			::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, item->len);
			OnAddRecvData(item);
		}	
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

	void NetManagerBase::Start(int port)
	{
		cout << "NetManagerBase::Start" << endl;
		JoinThread(m_Thread);
		m_IsRunning = true;
		m_Port = port;
		m_Thread = thread([](NetManagerBase *t){t->ThreadProcess(); }, this);
		m_InitSemaphore.wait();
	}

	void NetManagerBase::Stop()
	{
		cout << "NetManagerBase::Stop" << endl;
		m_IsRunning = false;
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

	void NetManagerBase::Send(UInt64 uid, int command, NetMessage *msg)
	{
		assert(uid != 0);
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DATA;
		item->uid = uid;
		item->cmd = command;
		item->len = msg->Pack(item->data, 0);
		m_SendQueue.Push(item);
	}

	void NetManagerBase::Send(const RemoteKey& key, int command, Byte *buffer, int len)
	{
		assert(key.first != RemoteType::RT_UNKNOW && (key.first != RemoteType::RT_CLIENT || key.second != 0));
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DATA;
		item->key = key;
		item->cmd = command;
		::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, len);
		item->len = len;
		m_SendQueue.Push(item);
	}

	void NetManagerBase::Send(const RemoteKey& key, int command, NetMessage *msg)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DATA;
		item->key = key;
		item->cmd = command;
		item->len = msg->Pack(item->data, 0);
		m_SendQueue.Push(item);
	}

	void NetManagerBase::SendToSelf(const RemoteKey& key, int command, NetMessage *msg)
	{
		NetDataItem *item = GetNetDataItem();
		item->op = NetOperateType::NOT_DATA;
		item->key = key;
		item->cmd = command;
		item->len = msg->Pack(item->data, 0);
		OnAddRecvData(item);
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

	void NetManagerBase::OnCreateInternalInfo(std::vector<int> &infos)
	{
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
			else
			{
				cout << "Unkonw command " << item->cmd << endl;
			}
		}
	}

	void NetManagerBase::ThreadProcess()
	{
		m_Server.SetProcesser(this);
		m_Server.Start(m_Port);
		m_InitSemaphore.post();

		//内部连接
		vector<int> infos;
		OnCreateInternalInfo(infos);
		for (vector<int>::iterator itr = infos.begin(); itr != infos.end(); ++itr)
		{
			ServerSetting *setting = StartSetting::GetInstance()->GetServerSetting(*itr);
			NetInternalConnection *con = new NetInternalConnection();
			con->Init(setting->GetType(), setting->GetIPAddress(), setting->GetPort());
			m_InternalConnections.insert(InternalConnectionMap::value_type(con->GetRomoteType(), con));
		}

		//网络循环
		while (m_IsRunning)
		{
			OnPostSend();
			m_Server.SelectSocket(100);		//阻塞等待时最多等100毫秒
			UpdateInternalConnection();
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
		//看是否发往内部连接
		if (item->key.first != RemoteType::RT_CLIENT)
		{
			InternalConnectionMap::iterator itr = m_InternalConnections.find(item->key.first);
			if (itr != m_InternalConnections.end())
			{
				NetInternalConnection *internal_con = itr->second;
				if (internal_con->GetState() == ConnectionState::CS_CONNECTED)
				{
					bool cansend = internal_con->Send(item->key.second, item->cmd, item->data, item->len);
					if (!cansend)
					{
						//实际上应该先缓存起来，等连接上了再发送
						cout << "Internal connection send failed! type:" << item->key.first << endl;
						internal_con->Close();
					}
				}
				else
				{
					//实际上应该先缓存起来，等连接上了再发送
					cout << "Internal connection is not connected! type:" << item->key.first << endl;
				}
				return;
			}
		}

		NetConnection *pcon = item->uid != 0 ? m_Server.GetConnection(item->uid) : m_Server.GetConnection(item->key);
		if (pcon != NULL)
		{
			bool cansend = pcon->Send(item->key.second, item->cmd, item->data, item->len);
			if (!cansend)
			{
				//同一网络周期内发送数据量过大或者该连接速度太低把数据缓冲区挤爆了，要断掉连接
				m_Server.CloseConnection(pcon);
				pcon = NULL;
				cout << "Connection send failed! key:" << item->key << endl;
			}
		}
	}

	void NetManagerBase::UpdateInternalConnection()
	{
		for (InternalConnectionMap::iterator itr = m_InternalConnections.begin(); itr != m_InternalConnections.end(); ++itr)
		{
			NetInternalConnection *con = itr->second;
			int oldstate = con->GetState();
			con->Select();
			int state = con->GetState();
			if (oldstate != state && (oldstate == ConnectionState::CS_CONNECTED || state == ConnectionState::CS_CONNECTED))
			{
				NetDataItem *item = GetNetDataItem();
				item->op = oldstate == ConnectionState::CS_CONNECTED ? NetOperateType::NOT_DISCONNECT : NetOperateType::NOT_CONNECT;
				item->key = con->GetRemote();
				m_RecvQueue.Push(item);

				if (state == ConnectionState::CS_CONNECTED)
				{
					//连上了发送身份识别
					static Byte data[NET_PACKAGE_MAX_SIZE];
					NetMessageInt req;					
					req.Value = MainBase::GetCurMain()->GetType();
					int len = req.Pack(data, 0);
					con->Send(0, NetMsgID::REMOTE_IDENTIFY, data, len);
				}
			}

			do
			{
				int ret = con->CheckRecvPackage();
				if (ret == 0)
				{
					break;
				}
				if (ret == -1)
				{
					con->Close();
					NetDataItem *item = GetNetDataItem();
					item->op = NetOperateType::NOT_DISCONNECT;
					item->uid = 0;
					item->key = con->GetRemote();
					m_RecvQueue.Push(item);
					break;
				}
				if (ret == 1)
				{
					//获取消息
					static Byte buffer[NET_PACKAGE_MAX_SIZE];
					NetPackageHeader header;
					con->TakeRecvPackage(header, buffer, NET_PACKAGE_MAX_SIZE);

					//加入消息队列
					NetDataItem *item = GetNetDataItem();
					item->op = NetOperateType::NOT_DATA;
					item->key = RemoteKey(con->GetRomoteType(), header.GUID);
					item->cmd = header.Command;
					item->len = header.BodySize;
					::memcpy_s(item->data, NET_PACKAGE_MAX_SIZE, buffer, item->len);
					OnAddRecvData(item);
				}
			} while (true);
		}
	}

	void NetManagerBase::OnMsgRemoteIdentify(NetConnection *connection, Int32 cmd, Byte *buffer, int len)
	{
		NetMessageInt msg;
		int index = msg.Unpack(buffer, 0);
		m_Server.SetRemote(connection->GetUniqueID(), RemoteKey(msg.Value, 0));
		cout << "OnMsgRemoteIdentify uid:" << connection->GetUniqueID() << " type:" << connection->GetRomoteType() << endl;
	}
}