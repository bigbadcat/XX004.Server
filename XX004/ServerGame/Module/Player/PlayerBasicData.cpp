/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerBasicData.cpp
* Summary: 玩家基础数据。
*
* Author: XuXiang
* Date: 2019-10-21 21:02
*******************************************************/

#include "PlayerBasicData.h"

namespace XX004
{
	const int PlayerBasicData::MODEL_ID = 1;

	PlayerBasicData::PlayerBasicData() : PlayerDataModel(MODEL_ID), m_ID(0), m_Prof(0), m_Level(0), m_Exp(0), m_Power(0)
	{

	}

	PlayerBasicData::~PlayerBasicData()
	{

	}
}