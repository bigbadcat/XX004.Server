/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetInternalConnection.h
* Summary: 网络通信模块服务内部连接。
*
* Author: XuXiang
* Date: 2018-11-26 14:49
*******************************************************/

#ifndef __NetInternalConnection_h__
#define __NetInternalConnection_h__

#include <map>
#include <mutex>
#include <iostream>
#include <WinSock2.h>
#include "../Macro.h"
#include "NetDefine.h"
#include "NetPackageHeader.h"
#include "../Util/ByteBuffer.h"

namespace XX004
{
	namespace Net
	{
		//连接状态
		enum ConnectionState
		{
			//未连接
			CS_NOT_CONNECTED = 0,

			//连接中
			CS_CONNECTING = 1,

			//已连接
			CS_CONNECTED = 2,

			//重试等待
			CS_RETRY_WAITTING = 3,
		};

		typedef std::pair<int, Int64> RemoteKey;

		class NetInternalConnection
		{
		public:
			NetInternalConnection();
			virtual ~NetInternalConnection();

			//初始化
			void Init(int type, const string &ip, int port);

			//关闭连接
			void Close();

			//获取连接状态
			inline int GetState()const { return m_State; }

			//获取Sokcet ret:Socket
			inline SOCKET GetSocket()const { return m_Socket; }

			//判断是否需要读数据
			inline bool IsNeedRead() { return NET_BUFFER_SIZE - m_RecvBuffer.GetLength() >= NET_PACKAGE_MAX_SIZE; }

			//判断是否需要写数据
			inline bool IsNeedWrite() { return m_SendBuffer.GetLength() > 0;}

			//获取远端标识
			inline RemoteKey GetRemote()const { return RemoteKey(m_RemoteType, 0); }

			//设置远端标识
			void SetRemote(const RemoteKey& key) { m_RemoteType = key.first; }

			//获取远端类型
			inline int GetRomoteType()const { return m_RemoteType; }

			//设置远端类型
			inline void SetRomoteType(int type) { m_RemoteType = type; }

			//获取IP地址
			inline const std::string& GetIPAddress()const { return m_IPAddress; }

			//获取端口号
			inline const int GetPort()const { return m_Port; }

			//发送数据
			bool Send(Int64 guid, Int32 cmd, Byte *buffer, int len);

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

			//选择IO操作
			void Select();

			//重试等待时间
			static const unsigned int RETRY_WAITTING_SECONDS = 5;

		private:

			//开始连接
			void StartConnect();

			//检测连接
			void CheckConnect();

			//检测输入输出
			void CheckIO();

			//Socket可以读取数据了
			int OnSocketRead();

			//Socket可以写入数据了
			int OnSocketWrite();

			//重试
			void Retry();


			//连接状态 0未连接 1连接中 2已连接
			int m_State;

			//远端类型
			int m_RemoteType;

			//Socket句柄
			SOCKET m_Socket;

			//IP地址。
			std::string m_IPAddress;

			//端口号。
			int m_Port;

			//下次重试时间
			UInt64 m_RetryTimestamp;

			//收发缓冲区
			ByteBuffer m_SendBuffer;

			//接收缓冲区
			ByteBuffer m_RecvBuffer;

			//接收的数据头
			NetPackageHeader m_RecvHeader;
		};
	}
}

#endif