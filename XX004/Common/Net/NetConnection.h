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
#include <mutex>
#include "../Macro.h"
#include "NetDefine.h"
#include "NetPackageHeader.h"
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

			//判断是否需要读数据
			virtual bool IsNeedRead() { return NET_BUFFER_SIZE - m_RecvLen >= NET_PACKAGE_MAX_SIZE; }

			//判断是否需要写数据
			virtual bool IsNeedWrite();

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

			//添加发送的数据
			bool AddSendData(Byte *buffer, int len);

			//添加接收的数据
			bool AddRecvData(Byte *buffer, int len);

			//检测是否接受到了完整数据包
			//ret:0 没有完整数据包 1:有完整数据包 -1:数据错误
			int CheckRecvPackage();

			//拿去接收到的数据包
			void TakeRecvPackage(NetPackageHeader& header, Byte *buffer, int size);

			//进行发送操作
			int DoSend();

		private:
			//远端标识
			RemoteKey m_Remote;

			//IP地址。
			std::string m_IPAddress;

			//端口号。
			int m_Port;

			//收发缓冲区
			Byte m_SendBuffer[NET_BUFFER_SIZE];

			//发送长度
			int m_SendLen;

			//发送锁
			std::mutex m_SendMutex;

			//接收缓冲区
			Byte m_RecvBuffer[NET_BUFFER_SIZE];

			//接收长度
			int m_RecvLen;

			//接收的数据头
			NetPackageHeader m_RecvHeader;
		};
	}
}

#endif	//__NetConnection_h__