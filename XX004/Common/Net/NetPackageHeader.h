/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetPackageHeader.h
* Summary: 数据包头。
*
* Author: XuXiang
* Date: 2018-07-30 23:08
*******************************************************/

#ifndef __NetPackageHeader_h__
#define __NetPackageHeader_h__

#include"../Macro.h"

namespace XX004
{
	namespace Net
	{
		//数据包头部。
		class NetPackageHeader
		{
		public:

			//数据包头签名始终为固定值0x5858。(XX)
			static const Int16 HEADER_SIGN = 0x5858;

			//包头大小。
			static const int HEADER_SIZE = 2 + 1 + 1 + 4 + 2 + 2 + 4 + 1 + 8 + 4;
			
			//包头签名。
			Int16 Sign;

			//随机种子，用来生成CheckSum。
			Byte RandSeed;

			//检验和, 生成见后文函数。
			Byte CheckSum;

			//压缩参数，0表示未压缩。
			Int32 Compress;

			//包的序列号，从0 ~9999 ，必须循环递增，用于底层处理断线重连。
			Int16 Index;

			//接收到的包的序列号。
			Int16 RecvIndex;

			//协议消息码。
			Int32 Command;

			//节点
			Byte Node;

			//玩家角色
			Int64 GUID;

			//包体大小（字节数）。
			Int32 BodySize;
		};
	}
}

#endif	//__NetPackageHeader_h__


/*

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace XuXiang.Net
{
/// <summary>
/// 数据包头部。
/// </summary>
public struct NetPackageHeader
{
#region 对外操作----------------------------------------------------------------

/// <summary>
/// 重置数据。
/// </summary>
public void Reset()
{
Sign = 0;
RandSeed = 0;
CheckSum = 0;
Compress = 0;
Index = 0;
Command = 0;
BodySize = 0;
}

/// <summary>
/// 解包头部。
/// </summary>
/// <param name="buffer">数据缓冲区。</param>
/// <param name="index">数据起始位置。</param>
public void Unpack(byte[] buffer, int index)
{
int i = index;
Sign = DataUtil.ReadInt16(buffer, i, ref i);
RandSeed = buffer[i++];
CheckSum = buffer[i++];
Compress = DataUtil.ReadInt32(buffer, i, ref i);
Index = DataUtil.ReadInt16(buffer, i, ref i);
Command = DataUtil.ReadInt16(buffer, i, ref i);
BodySize = DataUtil.ReadInt32(buffer, i, ref i);
}

/// <summary>
/// 封包头部。
/// </summary>
/// <param name="buffer">数据缓冲区。</param>
/// <param name="index">数据起始位置。</param>
public void Pack(byte[] buffer, int index)
{
int i = index;
i = DataUtil.WriteUInt16(buffer, i, (UInt16)HeaderSign);
buffer[i++] = RandSeed;
buffer[i++] = CheckSum;
i = DataUtil.WriteUInt32(buffer, i, (UInt32)Compress);
i = DataUtil.WriteUInt16(buffer, i, (UInt16)Index);
i = DataUtil.WriteUInt16(buffer, i, (UInt16)Command);
i = DataUtil.WriteUInt32(buffer, i, (UInt32)BodySize);
}

#endregion

*/