﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerData.h
* Summary: 数据服网络管理。
*
* Author: XuXiang
* Date: 2018-10-25 17:54
*******************************************************/

#ifndef __NetManagerData_h__
#define __NetManagerData_h__

#include "NetManagerBase.h"

namespace XX004
{
	class NetManagerData : public NetManagerBase
	{
	public:
		NetManagerData();
		virtual ~NetManagerData();

	protected:
		//创建内部连接信息
		virtual void OnCreateInternalInfo(std::vector<InternalInfo> &infos);

	private:

	};
}

#endif	//__NetManagerData_h__