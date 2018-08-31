/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: DataUtil.h
* Summary: ����ͨ�ò���������
*
* Author: XuXiang
* Date: 2018-08-26 22:23
*******************************************************/

#ifndef __DataUtil_h__
#define __DataUtil_h__

#include "../Macro.h"
#include <string>
using namespace std;

namespace XX004
{
	class DataUtil
	{
	public:

		//д��һ��64λ�޷���������
		//src:Ҫд����ֽ����顣
		//index:Ҫд���λ�á�
		//value:Ҫд���ֵ��
		//ret:��һ��д��λ�á�
		static int WriteUInt64(Byte *src, int index, UInt64 value);

		//д��һ��64λ������
		//src:Ҫд����ֽ����顣
		//index:Ҫд���λ�á�
		//value:Ҫд���ֵ��
		//ret:��һ��д��λ�á�
		static inline int WriteInt64(Byte *src, int index, Int64 value) { return WriteUInt64(src, index, (UInt64)value); }

		//��ȡһ��64λ�޷��������� 
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//ret:��ȡ��ֵ��
		static UInt64 ReadUInt64(Byte *src, int index, int *next);

		//��ȡһ��64λ������ 
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//ret:��ȡ��ֵ��
		static inline Int64 ReadInt64(Byte *src, int index, int *next) { return (Int64)ReadUInt64(src, index, next); }

		//д��һ��32λ�޷���������
		//src:Ҫд����ֽ����顣
		//index:Ҫд���λ�á�
		//value:Ҫд���ֵ��
		//ret:��һ��д��λ�á�
		static int WriteUInt32(Byte *src, int index, UInt32 value);

		//д��һ��32λ������
		//src:Ҫд����ֽ����顣
		//index:Ҫд���λ�á�
		//value:Ҫд���ֵ��
		//ret:��һ��д��λ�á�
		static inline int WriteInt32(Byte *src, int index, Int32 value) { return WriteUInt32(src, index, (UInt32)value); }

		//��ȡһ��32λ�޷��������� 
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//ret:��ȡ��ֵ��
		static UInt32 ReadUInt32(Byte *src, int index, int *next);

		//��ȡһ��32λ������ 
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//ret:��ȡ��ֵ��
		static inline Int32 ReadInt32(Byte *src, int index, int *next) { return (Int32)ReadUInt32(src, index, next); }

		//д��һ��16λ�޷���������
		//src:Ҫд����ֽ����顣
		//index:Ҫд���λ�á�
		//value:Ҫд���ֵ��
		//ret:��һ��д��λ�á�
		static int WriteUInt16(Byte *src, int index, UInt16 value);

		//д��һ��16λ������
		//src:Ҫд����ֽ����顣
		//index:Ҫд���λ�á�
		//value:Ҫд���ֵ��
		//ret:��һ��д��λ�á�
		static inline int WriteInt16(Byte *src, int index, Int16 value) { return WriteUInt16(src, index, (UInt16)value); }

		//��ȡһ��16λ�޷��������� 
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//ret:��ȡ��ֵ��
		static UInt16 ReadUInt16(Byte *src, int index, int *next);

		//��ȡһ��16λ������ 
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//ret:��ȡ��ֵ��
		static inline Int16 ReadInt16(Byte *src, int index, int *next) { return (Int16)ReadUInt16(src, index, next); }

		//д��һ����������
		//src:Ҫд����ֽ����顣
		//index:Ҫд���λ�á�
		//value:Ҫд���ֵ��
		//ret:��һ��д��λ�á�
		static int WriteFloat(Byte *src, int index, float value);

		//��ȡһ����������
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//ret:��ȡ��ֵ��
		static float ReadFloat(Byte *src, int index, int *next);

		//д��һ��˫���ȸ�������
		//src:Ҫд����ֽ����顣
		//index:Ҫд���λ�á�
		//value:Ҫд���ֵ��
		//ret:��һ��д��λ�á�
		static int WriteDouble(Byte *src, int index, double value);

		//��ȡһ��˫���ȸ�������
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//ret:��ȡ��ֵ��
		static double ReadDouble(Byte *src, int index, int *next);

		//д��һ���ַ�����
		//Ҫд����ֽ����顣
		//Ҫд�����ʼλ�á�
		//Ҫд���ֵ��
		//д�볤�ȡ�
		//�µ���ʼλ�á�
		static int WriteString(Byte *src, int index, const string& value, int n = 0);

		//��ȡһ��˫���ȸ�������
		//src:Ҫ��ȡ���ֽ����顣
		//index:Ҫ��ȡ��λ�á�
		//next:[out]��һ����ȡλ�á�
		//��ȡ���ȡ�
		//ret:��ȡ��ֵ��
		static string ReadString(Byte *src, int index, int *next, int n = 0);
	};
}

#endif	//__DataUtil_h__