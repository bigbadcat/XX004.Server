/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMessage.cpp
* Summary: 网络消息基类。
*
* Author: XuXiang
* Date: 2018-10-30 20:29
*******************************************************/

#include "NetMessage.h"
#include <iostream>
using namespace std;

namespace XX004
{
	namespace Net
	{
		NetMessage::NetMessage()
		{
		}
		
		NetMessage::~NetMessage()
		{
		}

		int NetMessage::Unpack(Byte *buffer, int index)
		{
			return index;
		}

		int NetMessage::Pack(Byte *buffer, int index)
		{
			return index;
		}

		//----------------------------------------

		int NetMessageInt::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Value = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		int NetMessageInt::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Value);
			return i;
		}

		//----------------------------------------

		int NetMessageIntInt::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Value1 = DataUtil::ReadInt32(buffer, i, &i);
			Value2 = DataUtil::ReadInt32(buffer, i, &i);
			return i;
		}

		int NetMessageIntInt::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Value1);
			i = DataUtil::WriteInt32(buffer, i, Value2);
			return i;
		}

		//----------------------------------------

		int NetMessageInt64::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Value = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int NetMessageInt64::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt64(buffer, i, Value);
			return i;
		}

		//----------------------------------------

		int NetMessageIntInt64::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Value1 = DataUtil::ReadInt32(buffer, i, &i);
			Value2 = DataUtil::ReadInt64(buffer, i, &i);
			return i;
		}

		int NetMessageIntInt64::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Value1);
			i = DataUtil::WriteInt64(buffer, i, Value2);
			return i;
		}

		//----------------------------------------

		int NetMessageString::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Value = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		int NetMessageString::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteString(buffer, i, Value);
			return i;
		}

		//----------------------------------------

		int NetMessageIntString::Unpack(Byte *buffer, int index)
		{
			int i = index;
			Value1 = DataUtil::ReadInt32(buffer, i, &i);
			Value2 = DataUtil::ReadString(buffer, i, &i);
			return i;
		}

		int NetMessageIntString::Pack(Byte *buffer, int index)
		{
			int i = index;
			i = DataUtil::WriteInt32(buffer, i, Value1);
			i = DataUtil::WriteString(buffer, i, Value2);
			return i;
		}
	}
}