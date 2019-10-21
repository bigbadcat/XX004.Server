/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: DataModel.h
* Summary: 数据公共基类。
*
* Author: XuXiang
* Date: 2019-10-21 11:53
*******************************************************/

#ifndef __DataModel_h__
#define __DataModel_h__

#include "AttrSet.h"
#include <vector>
using namespace std;

namespace XX004
{
	class DataModel
	{
		typedef vector<DataModel*> DataModelVector;
	public:
		DataModel(int mid);
		virtual ~DataModel();

		//设置自身属性需要重新计算标记
		void SetSelfDirty();

		//设置子节点属性需要重新计算标记
		inline void SetChildDirty() { m_ChildDirty = true; }

		//获取父节点
		inline DataModel* GetParent() { return m_Parent; }

		//获取自身属性
		const AttrSet& GetSelfAttr();

		//获取所有属性
		const AttrSet& GetAllAttr();

		//获取模型ID
		inline int GetModelID()const { return m_ModelID; }

		//添加模型
		virtual void AddModel(DataModel *model);

		//获取数据模型 model_id:模型编号 only_top:是否只搜索直接子节点
		DataModel* GetModel(int model_id, bool only_top = false);

		//获取更节点模型
		DataModel* GetRoot();

	protected:

		//需要重建自生属性
		virtual void OnBuildSelfAttr() {}

		//设置父节点
		inline void SetParent(DataModel *parent) { m_Parent = parent; }

	private:

		//模型标识
		int m_ModelID;

		//自身的属性集合
		AttrSet m_SelfAttr;

		//自身加所有子节点的属性集合
		AttrSet m_AllAttr;

		//自身属性需要重新计算
		bool m_SelfDirty;

		//子节点属性需要重新计算
		bool m_ChildDirty;

		//父节点
		DataModel* m_Parent;

		//字节点
		DataModelVector m_Children;
	};
}

#endif