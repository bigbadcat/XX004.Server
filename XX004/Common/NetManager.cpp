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
#include "Util/DataUtil.h"
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

	void NetManager::Start(const string &ipaddress, int port)
	{
		m_Server.SetProcesser(this);
		m_Server.Start(ipaddress, port);
	}

	void NetManager::Stop()
	{
		m_Server.Stop();
	}

	void NetManager::OnConnected(NetConnection *connection)
	{
		cout << "NetManager::OnConnected ip:" << connection->GetIPAddress() << " port:" << connection->GetPort() << endl;
	}

	void NetManager::OnDisconnected(NetConnection *connection)
	{
		cout << "NetManager::OnDisconnected ip:" << connection->GetIPAddress() << " port:" << connection->GetPort() << endl;
	}

	void NetManager::OnRecvData(NetConnection *connection, const NetPackageHeader& header, Byte *buffer)
	{
		//解析网络消息并添加到队列中
		cout << "NetManager::OnRecvData ip:" << connection->GetIPAddress() << " port:" << connection->GetPort() << endl;
		cout << " cmd:" << header.Command << " len:" << header.BodySize << endl;
		int i = 0;
		string text = DataUtil::ReadString(buffer, i, &i);
		cout << " text:" << text << endl;

		//NetPackageHeader recvhead;
		//int i = 0;
		//i = recvhead.Unpack(m_RecvBuffer, i);
		//string text = DataUtil::ReadString(m_RecvBuffer, i, &i);
		//cout << "text:" << text << endl;

		//准备回复数据							
		//Byte recvdata[1024];
		//int index = 0;

		//int result = 4;
		//index = DataUtil::WriteInt32(recvdata, index, result);
		//string ret = text + "吼吼吼吼";
		//index = DataUtil::WriteString(recvdata, index, ret);
		//Int64 freetime = 0;
		//index = DataUtil::WriteInt64(recvdata, index, freetime);

		////回复的数据头
		//NetPackageHeader sendhead(recvhead);
		//sendhead.Command = 1050;
		//sendhead.BodySize = index;

		//sendsize = sendhead.Pack(sendbuff + sendsize, sendsize);
		//::memcpy_s(sendbuff + sendsize, buffsize - sendsize, recvdata, index);
		//sendsize += index;
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
		//分发接收消息队列

		//写入发送消息队列
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