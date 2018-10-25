#include "ServerData.h"
#include "NetManagerData.h"
#include "ServerMain.h"

namespace XX004
{
	ServerData::ServerData()
	{
	}

	ServerData::~ServerData()
	{
	}

	void ServerData::RegisterNetMessage(NetManager *pMgr)
	{
		pMgr->RegisterMessageCallBack(1, [this](Int32 cmd, Byte *buffer, int len){this->F1(cmd, buffer, len); });
		pMgr->RegisterMessageCallBack(2, [this](Int32 cmd, Byte *buffer, int len){this->F2(cmd, buffer, len); });
	}

	bool ServerData::OnStart(int step, float &r)
	{
		ServerMain::GetCurMain()->GetNetManager()->Test(2);
		r = 1;
		return true;
	}

	void ServerData::OnUpdate()
	{

	}

	void ServerData::OnUpdatePerSecond()
	{

	}

	bool ServerData::OnStop(int step, float &r)
	{
		r = 1;
		return true;
	}

	void ServerData::F1(Int32 cmd, Byte *buffer, int len)
	{
		cout << "F1" << endl;
	}

	void ServerData::F2(Int32 cmd, Byte *buffer, int len)
	{
		cout << "F2" << endl;
	}
}