﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ByteBuffer.cpp
* Summary: 字节缓冲区。
*
* Author: XuXiang
* Date: 2018-11-24 12:15
*******************************************************/

#include "ByteBuffer.h"
#include <assert.h>
#include <cstring>

namespace XX004
{
	ByteBuffer::ByteBuffer(int size) : m_pData(NULL), m_Size(size), m_Length(0)
	{
		assert(m_Size > 0);
		m_pData = new Byte[m_Size];
	}

	ByteBuffer::~ByteBuffer()
	{
		SAFE_DELETE_ARRAY(m_pData);
		m_Size = 0;
		m_Length = 0;
	}

	bool ByteBuffer::AddData(Byte* data, int len)
	{
		if (m_Length + len > m_Size)
		{
			return false;
		}

		::memcpy(m_pData + m_Length, data, len);
		m_Length += len;
		return true;
	}

	void ByteBuffer::GetData(Byte* dst, int index, int len)
	{
		assert(index + len <= m_Length);		//保证有足够的数据
		::memcpy(dst, m_pData + index, len);
	}

	void ByteBuffer::RemoveData(int len)
	{
		assert(len <= m_Length);
		m_Length -= len;
		if (m_Length > 0)
		{
			//剩余数据回移
			::memcpy(m_pData, m_pData + len, m_Length);
		}		
	}
}