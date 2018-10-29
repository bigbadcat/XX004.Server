/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: semaphore.h
* Summary: 线程信号。
*
* Author: XuXiang
* Date: 2018-10-27 15:54
*******************************************************/

#ifndef __semaphore_h__
#define __semaphore_h__

#include <mutex>
#include <condition_variable>

namespace xxstd
{
	class semaphore
	{
	public:
		//构造函数
		semaphore(int init = 0);

		//等待并消耗一个信号，若没有信号则线程挂起，直到信号到来
		void wait();

		//尝试等待并消耗一个信号，线程不会挂起
		//return:true有信号 false没信号
		bool try_wait();

		//提交一个信号
		void post();

	private:

		std::condition_variable m_cv;
		std::mutex m_mutex;
		int m_value;
	};
}

#endif	//__semaphore_h__