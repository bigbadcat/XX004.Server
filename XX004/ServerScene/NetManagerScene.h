/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerScene.h
* Summary: 场景服网络管理。
*
* Author: XuXiang
* Date: 2019-09-21 16:54
*******************************************************/

#ifndef __NetManagerScene_h__
#define __NetManagerScene_h__

#include "NetManagerBase.h"

namespace XX004
{
	class NetManagerScene : public NetManagerBase
	{
	public:
		NetManagerScene();
		virtual ~NetManagerScene();

	protected:
		//创建内部连接信息
		virtual void OnCreateInternalInfo(std::vector<int> &infos);

	private:

	};
}

#endif