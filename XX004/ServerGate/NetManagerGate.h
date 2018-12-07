/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerrGate.h
* Summary: 网关网络管理。
*
* Author: XuXiang
* Date: 2018-12-03 19:42
*******************************************************/

#ifndef __NetManagerGate_h__
#define __NetManagerGate_h__

#include "NetManagerBase.h"

namespace XX004
{
	class NetManagerGate : public NetManagerBase
	{
	public:
		NetManagerGate();
		virtual ~NetManagerGate();

	protected:
		//创建内部连接信息
		virtual void OnCreateInternalInfo(std::vector<int> &infos);

	private:

	};
}

#endif	//__NetManagerGate_h__