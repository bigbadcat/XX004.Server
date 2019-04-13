/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerGate.cpp
* Summary: 网关网络管理。
*
* Author: XuXiang
* Date: 2018-12-03 19:42
*******************************************************/

#include "NetManagerGate.h"
#include <Protocol/NetMsgID.h>

namespace XX004
{
	GateRouteConfig::GateRouteConfig(int b, int e, RemoteType t) : m_Begin(b), m_End(e), m_Target(t)
	{
	}

	NetManagerGate::NetManagerGate()
	{
		m_GateRouteConfig.push_back(GateRouteConfig(NetMsgID::LOGIN_BEGIN, NetMsgID::LOGIN_END, RemoteType::RT_LOGIN));
		m_GateRouteConfig.push_back(GateRouteConfig(NetMsgID::SCENE_BEGIN, NetMsgID::SCENE_END, RemoteType::RT_SCENE));
		m_GateRouteConfig.push_back(GateRouteConfig(NetMsgID::WORLD_BEGIN, NetMsgID::WORLD_END, RemoteType::RT_WORLD));
		m_GateRouteConfig.push_back(GateRouteConfig(NetMsgID::CLIENT_BEGIN, NetMsgID::CLIENT_END, RemoteType::RT_CLIENT));
	}

	NetManagerGate::~NetManagerGate()
	{
	}

	void NetManagerGate::OnAddRecvData(NetDataItem *item)
	{
		if (item->op == NetOperateType::NOT_DATA)
		{
			for (GateRouteConfigVector::iterator itr = m_GateRouteConfig.begin(); itr != m_GateRouteConfig.end(); ++itr)
			{
				if (item->cmd > itr->GetBegin() && item->cmd < itr->GetEnd())
				{
					//转发
					Send(RemoteKey(itr->GetTarget(), item->key.second), item->cmd, item->data, item->len);
					CacheNetDataItem(item);
					return;
				}
			}
		}
		
		NetManagerBase::OnAddRecvData(item);
	}

	void NetManagerGate::OnCreateInternalInfo(std::vector<int> &infos)
	{
		infos.push_back(RemoteType::RT_LOGIN);
		infos.push_back(RemoteType::RT_WORLD);
	}
}