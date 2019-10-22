/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: PlayerDataModel.cpp
* Summary: 玩家数据模型。
*
* Author: XuXiang
* Date: 2019-10-21 20:41
*******************************************************/

#include "PlayerDataModel.h"
#include "PlayerBasicData.h"
#include <assert.h>

namespace XX004
{
	PlayerDataModel::PlayerDataModel(int mid) : DataModel(mid), m_Owner(NULL)
	{

	}

	PlayerDataModel::~PlayerDataModel()
	{
		m_Owner = NULL;
	}

	void PlayerDataModel::AddModel(DataModel *model)
	{
		//判断是不是PlayerDataModel
		PlayerDataModel *player = dynamic_cast<PlayerDataModel*>(model);
		assert(player != NULL);
		DataModel::AddModel(model);
	}

	void PlayerDataModel::OnAddToModel(DataModel *parent)
	{
		//如果父节点没有owner，说明父节点自己就是owner
		PlayerDataModel *player = dynamic_cast<PlayerDataModel*>(parent);
		assert(player != NULL);
		m_Owner = player->GetOwner() == NULL ? dynamic_cast<PlayerBasicData*>(player) : player->GetOwner();
	}
}