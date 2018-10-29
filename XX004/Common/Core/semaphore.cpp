/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: semaphore.cpp
* Summary: 线程信号。
*
* Author: XuXiang
* Date: 2018-10-27 15:54
*******************************************************/

#include "semaphore.h"
#include <algorithm>
#include <assert.h>

namespace xxstd
{
	semaphore::semaphore(int init) : m_value(init)
	{
	}

	void semaphore::wait()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while (m_value <= 0)
		{
			m_cv.wait(lock);		//线程被阻塞并挂起到条件变量的线程队列中，且互斥锁(lock)被释放(重要！重要！重要！)
		}
	}

	bool semaphore::try_wait()
	{
		std::unique_lock<std::mutex> lock(m_mutex);		//调用wait函数的线程在m_cv.wait挂起时，m_mutex互斥锁已被释放
		if (m_value <= 0)
		{
			return false;
		}
		--m_value;
		return true;
	}

	void semaphore::post()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			++m_value;
		}
		m_cv.notify_one();			//notify的时候需要解除互斥锁
	}
}