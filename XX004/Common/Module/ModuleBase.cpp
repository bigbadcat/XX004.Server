/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ModuleBase.cpp
* Summary: 模块基类。
*
* Author: XuXiang
* Date: 2019-10-03 11:37
*******************************************************/

#include "ModuleBase.h"
#include "../ServerBase.h"
#include "../StorageManager.h"
#include "../MainBase.h"

namespace XX004
{
	ModuleBase::ModuleBase()
	{
	}

	ModuleBase::~ModuleBase()
	{
	}

	void ModuleBase::Init()
	{
	}

	void ModuleBase::Release()
	{
	}

	ServerBase* ModuleBase::GetServer()
	{
		return MainBase::GetCurMain()->GetServer();
	}

	MySQLWrap* ModuleBase::GetMySQL()
	{
		return MainBase::GetCurMain()->GetStorageManager()->GetMySQL();
	}

	void ModuleBase::SendNet(Int64 rid, int command, Net::NetMessage *msg)
	{
		RemoteKey key(RemoteType::RT_CLIENT, rid);
		MainBase::GetCurMain()->GetNetManager()->Send(key, command, msg);
	}

	void ModuleBase::RequestStorage(Int64 rid, int command, Net::NetMessage *msg)
	{
		MainBase::GetCurMain()->GetStorageManager()->Request(rid, command, msg);
	}

	void ModuleBase::ResponseStorage(Int64 rid, int command, Net::NetMessage *msg)
	{
		RemoteKey key(MainBase::GetCurMain()->GetType(), rid);
		MainBase::GetCurMain()->GetNetManager()->SendToSelf(key, command, msg);
	}
}