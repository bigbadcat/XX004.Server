/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerWorld.h
* Summary: 世界服网络管理。
*
* Author: XuXiang
* Date: 2019-04-12 16:43
*******************************************************/

#ifndef __NetManagerWorld_h__
#define __NetManagerWorld_h__

#include "NetManagerBase.h"

namespace XX004
{
	class NetManagerWorld : public NetManagerBase
	{
	public:
		NetManagerWorld();
		virtual ~NetManagerWorld();

	protected:
		//创建内部连接信息
		virtual void OnCreateInternalInfo(std::vector<int> &infos);

	private:

	};
}

#endif