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
	class PlayerBasicData;

	//玩家部分的数据都继承此类
	class PlayerDataModel : public DataModel
	{
	public:
		PlayerDataModel(int mid);
		virtual ~PlayerDataModel();

		//获取所属玩家
		inline PlayerBasicData* GetOwner() { return m_Owner; }

		//添加模型
		virtual void AddModel(DataModel *model);

	protected:

		//添加到了其它模块里
		virtual void OnAddToModel(DataModel *parent);

	private:

		//模型所属玩家
		PlayerBasicData* m_Owner;
	};
}

#endif