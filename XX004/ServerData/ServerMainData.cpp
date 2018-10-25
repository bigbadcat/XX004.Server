#include "ServerMainData.h"
#include "NetManagerData.h"
#include "ServerData.h"

namespace XX004
{
	ServerMainData::ServerMainData()
	{
	}

	ServerMainData::~ServerMainData()
	{
	}

	NetManager* ServerMainData::OnCreateNetManager()
	{
		return new NetManagerData();
	}

	ServerBase* ServerMainData::OnCreateServer()
	{
		return new ServerData();
	}
}