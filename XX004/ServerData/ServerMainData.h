#ifndef __ServerMainData_h__
#define __ServerMainData_h__

#include "ServerMain.h"

namespace XX004
{
	class ServerMainData : public ServerMain
	{
	public:
		ServerMainData();
		virtual ~ServerMainData();

	protected:
		//创建网络管理
		virtual NetManager* OnCreateNetManager();

		//创建服务模块
		virtual ServerBase* OnCreateServer();

	private:

	};
}

#endif	//__ServerMainData_h__