/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnection.h
* Summary: 网络通信模块服务端连接。
*
* Author: XuXiang
* Date: 2018-07-30 23:06
*******************************************************/

#ifndef __NetConnection_h__
#define __NetConnection_h__

#include <map>
#include "../Macro.h"
#include "NetSocketThread.h"

namespace XX004
{
	namespace Net
	{
		//远端唯一标识。
		typedef std::pair<int, Int64> RemoteKey;

		//远端类型。
		enum RemoteType
		{
			//未知
			RT_UNKNOW = 0,

			//玩家客户端。
			RT_CLIENT = 1,

			//网关
			RT_GATE = 2,

			//登陆服
			RT_LOGIN = 3,

			//数据服
			RT_DATA = 4,

			//世界服
			RT_WORLD = 5,

			//场景服
			RT_SCENE = 6,
		};

		//网络连接对象
		class NetConnection : public NetSocketWrap
		{
		public:
			NetConnection();
			virtual ~NetConnection();

			//设置Socket
			virtual void SetSocket(SOCKET s);

			//获取远端标识
			inline const RemoteKey& GetRemote()const { return m_Remote; }

			//设置远端标识
			inline void SetRemote(const RemoteKey& key) { m_Remote = key; }

			//获取IP地址
			inline const std::string& GetIPAddress()const { return m_IPAddress; }

			//获取端口号
			inline const int GetPort()const { return m_Port; }

		private:
			//远端标识
			RemoteKey m_Remote;

			//IP地址。
			std::string m_IPAddress;

			//端口号。
			int m_Port;
		};
	}
}

#endif	//__NetConnection_h__