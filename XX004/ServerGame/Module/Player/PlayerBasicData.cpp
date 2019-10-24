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
#include <Config/BasicModuleConfig.h>

namespace XX004
{
	const int PlayerBasicData::MODEL_ID = 1;

	PlayerBasicData::PlayerBasicData() : PlayerDataModel(MODEL_ID), m_ID(0), m_Prof(0), m_Level(0), m_Exp(0), m_Power(0),
		m_PosMap(0), m_PosX(0), m_PosY(0), m_PosDir(0)
	{

	}

	PlayerBasicData::~PlayerBasicData()
	{
	}

	void PlayerBasicData::UpdatePosition(int map, int x, int y, int dir)
	{
		m_PosMap = map;
		m_PosX = x;
		m_PosY = y;
		m_PosDir = dir;
	}

	void PlayerBasicData::OnBuildSelfAttr()
	{
		BasicModuleConfig *basic_cfg = ModuleConfig::GetInstance<BasicModuleConfig>();
		m_SelfAttr.Clear();
		m_SelfAttr.AddAttr(basic_cfg->GetProfAttr(m_Prof, m_Level));
	}
}