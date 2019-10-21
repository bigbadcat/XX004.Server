/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerBasicData.h
* Summary: 玩家基础数据。
*
* Author: XuXiang
* Date: 2019-10-21 21:02
*******************************************************/

#ifndef __PlayerBasicData_h__
#define __PlayerBasicData_h__

#include "PlayerDataModel.h"

namespace XX004
{
	//玩家部分的数据都继承此类
	class PlayerBasicData: public PlayerDataModel
	{
	public:
		PlayerBasicData();
		virtual ~PlayerBasicData();

		static const int MODEL_ID;
	};
}

#endif