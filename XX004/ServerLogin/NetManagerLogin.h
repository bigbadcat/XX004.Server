/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerLogin.h
* Summary: 登陆网络管理。
*
* Author: XuXiang
* Date: 2018-12-18 19:56
*******************************************************/

#ifndef __NetManagerLogin_h__
#define __NetManagerLogin_h__

#include "NetManagerBase.h"

namespace XX004
{
	class NetManagerLogin : public NetManagerBase
	{
	public:
		NetManagerLogin();
		virtual ~NetManagerLogin();

	protected:
		//创建内部连接信息
		virtual void OnCreateInternalInfo(std::vector<int> &infos);

	private:

	};
}

#endif