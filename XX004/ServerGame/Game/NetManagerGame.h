/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerGame.h
* Summary: 游戏服网络管理。
*
* Author: XuXiang
* Date: 2019-10-02 11:05
*******************************************************/

#ifndef __NetManagerGame_h__
#define __NetManagerGame_h__

#include <Frame/NetManagerBase.h>

namespace XX004
{
	class NetManagerGame : public NetManagerBase
	{
	public:
		NetManagerGame();
		virtual ~NetManagerGame();

	protected:
		//创建内部连接信息
		virtual void OnCreateInternalInfo(std::vector<int> &infos);

	private:

	};
}

#endif