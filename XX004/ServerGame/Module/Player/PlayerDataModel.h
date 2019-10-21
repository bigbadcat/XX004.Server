/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerDataModel.h
* Summary: 玩家数据模型。
*
* Author: XuXiang
* Date: 2019-10-21 20:41
*******************************************************/

#ifndef __PlayerDataModel_h__
#define __PlayerDataModel_h__

#include <Module/Model/DataModel.h>

namespace XX004
{
	//玩家部分的数据都继承此类
	class PlayerDataModel : public DataModel
	{
	public:
		PlayerDataModel(int mid);
		virtual ~PlayerDataModel();
	};
}

#endif