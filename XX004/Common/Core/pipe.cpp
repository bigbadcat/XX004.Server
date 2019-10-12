/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: pipe.cpp
* Summary: 管道封装。
*
* Author: XuXiang
* Date: 2019-10-11 11:49
*******************************************************/

#include "pipe.h"
#if  WIN32
#include <windows.h>
#else
#endif

namespace xxstd
{
#if  WIN32
	//windows下的管道实现
	pipe::pipe()
	{
		m_fd[0] = 0;
		m_fd[1] = 0;

		HANDLE hr, hw;
		if (::CreatePipe(&hr, &hw, NULL, 64))
		{
			m_fd[0] = hr;
			m_fd[1] = hw;
		}
	}

	pipe::~pipe()
	{
		if (m_fd[0] != 0)
		{
			::CloseHandle(m_fd[0]);
			m_fd[0] = 0;
		}
		if (m_fd[1] != 0)
		{
			::CloseHandle(m_fd[1]);
			m_fd[1] = 0;
		}
	}

	bool pipe::read(char *buffer, int size)
	{
		if (get_read() == 0)
		{
			return false;
		}

		HANDLE hr = (HANDLE)get_read();
		return ::ReadFile(hr, buffer, size, NULL, NULL) != 0;
	}

	bool pipe::write(const char *buffer, int len)
	{
		if (get_write() == 0)
		{
			return false;
		}

		HANDLE hw = (HANDLE)get_write();
		return ::WriteFile(hw, buffer, len, NULL, NULL) != 0;
	}



#else
	//linux下的管道实现
#endif
}