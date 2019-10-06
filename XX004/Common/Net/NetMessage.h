/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetMessage.h
* Summary: 网络消息基类。
*
* Author: XuXiang
* Date: 2018-10-30 20:29
*******************************************************/

#ifndef __NetMessage_h__
#define __NetMessage_h__

#include "../Macro.h"
#include "../Util/DataUtil.h"
#include <string>

namespace XX004
{
	namespace Net
	{
		class NetMessage
		{
		public:
			//构造析构函数
			NetMessage();
			virtual ~NetMessage();

			//解包消息。
			//src:要读取的字节数组。
			//index:要读取的位置。
			//ret:下一个读取位置。
			virtual int Unpack(Byte *buffer, int index);

			//打包消息。
			//src:要写入的字节数组。
			//index:要写入的位置。
			//ret:下一个写入位置。
			virtual int Pack(Byte *buffer, int index);
		};

		//通用空消息体
		class NetMessageEmpty : public NetMessage
		{
		};

		//通用消息体-Int
		class NetMessageInt : public NetMessage
		{
		public:

			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int32 Value;
		};

		//通用消息体-IntInt
		class NetMessageIntInt : public NetMessage
		{
		public:

			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int32 Value1;
			Int32 Value2;
		};

		//通用消息体-Int64
		class NetMessageInt64 : public NetMessage
		{
		public:

			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int64 Value;
		};

		//通用消息体-IntInt64
		class NetMessageIntInt64 : public NetMessage
		{
		public:

			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int32 Value1;
			Int64 Value2;
		};

		//通用消息体-String
		class NetMessageString : public NetMessage
		{
		public:

			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			std::string Value;
		};

		//通用消息体-IntString
		class NetMessageIntString : public NetMessage
		{
		public:

			virtual int Unpack(Byte *buffer, int index);
			virtual int Pack(Byte *buffer, int index);

			Int32 Value1;
			std::string Value2;
		};
	}
}

#endif