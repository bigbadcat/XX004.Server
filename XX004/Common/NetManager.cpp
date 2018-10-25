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

	void NetManager::Test(Int32 cmd)
	{
		MessageCallBackMap::iterator itr = m_CallBack.find(cmd);
		if (itr != m_CallBack.end())
		{
			(itr->second)(cmd, NULL, 0);
		}
	}
}