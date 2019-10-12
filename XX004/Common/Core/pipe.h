/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: pipe.h
* Summary: 管道封装。
*
* Author: XuXiang
* Date: 2019-10-11 11:49
*******************************************************/

#ifndef __pipe_h__
#define __pipe_h__

#include <mutex>
#include <condition_variable>

namespace xxstd
{
	typedef void *fd;

	class pipe
	{
	public:
		pipe();
		~pipe();

		//获取读文件句柄
		inline fd get_read()const { return m_fd[0]; }

		//获取写文件句柄
		inline fd get_write()const { return m_fd[1]; }

		//从管道中读取数据
		bool read(char *buffer, int size);

		//向管道中写入数据。
		bool write(const char *buffer, int len);

	private:
		fd m_fd[2];

	};
}

#endif