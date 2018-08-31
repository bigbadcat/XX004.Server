/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetPackageHeader.h
* Summary: ���ݰ�ͷ��
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
		//���ݰ�ͷ����
		class NetPackageHeader
		{
		public:

			//���ݰ�ͷǩ��ʼ��Ϊ�̶�ֵ0x5858��(XX)
			static const Int16 HEADER_SIGN = 0x5858;

			//��ͷ��С��
			static const int HEADER_SIZE = 2 + 1 + 1 + 4 + 2 + 2 + 4 + 1 + 8 + 4;
			
			//��ͷǩ����
			Int16 Sign;

			//������ӣ���������CheckSum��
			Byte RandSeed;

			//�����, ���ɼ����ĺ�����
			Byte CheckSum;

			//ѹ��������0��ʾδѹ����
			Int32 Compress;

			//�������кţ���0 ~9999 ������ѭ�����������ڵײ㴦�����������
			Int16 Index;

			//���յ��İ������кš�
			Int16 RecvIndex;

			//Э����Ϣ�롣
			Int32 Command;

			//�ڵ�
			Byte Node;

			//��ҽ�ɫ
			Int64 GUID;

			//�����С���ֽ�������
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
/// ���ݰ�ͷ����
/// </summary>
public struct NetPackageHeader
{
#region �������----------------------------------------------------------------

/// <summary>
/// �������ݡ�
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
/// ���ͷ����
/// </summary>
/// <param name="buffer">���ݻ�������</param>
/// <param name="index">������ʼλ�á�</param>
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
/// ���ͷ����
/// </summary>
/// <param name="buffer">���ݻ�������</param>
/// <param name="index">������ʼλ�á�</param>
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