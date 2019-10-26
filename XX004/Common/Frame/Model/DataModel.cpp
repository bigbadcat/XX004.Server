/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: DataModel.cpp
* Summary: 数据公共基类。
*
* Author: XuXiang
* Date: 2019-10-21 11:53
*******************************************************/

#include "DataModel.h"
#include <assert.h>

namespace XX004
{
	DataModel::DataModel(int mid) : m_ModelID(mid), m_SelfDirty(false), m_ChildDirty(false), m_Parent(NULL)
	{
	}

	DataModel::~DataModel()
	{
		m_Parent = NULL;
		SAFE_DELETE_VECTOR(m_Children);
	}

	void DataModel::SetSelfDirty()
	{
		m_SelfDirty = true;
		if (m_Parent != NULL)
		{
			m_Parent->SetChildDirty();
		}
	}

	void DataModel::SetChildDirty()
	{
		m_ChildDirty = true;
		DataModel* parent = m_Parent;
		while (parent != NULL)
		{
			parent->m_ChildDirty = true;
			parent = parent->GetParent();
		}
	}

	const AttrSet& DataModel::GetSelfAttr()
	{
		if (m_SelfDirty)
		{
			m_SelfDirty = false;
			OnBuildSelfAttr();
		}
		return m_SelfAttr;
	}

	const AttrSet& DataModel::GetAllAttr()
	{
		if (m_SelfDirty || m_ChildDirty)
		{
			OnBuildAllAttr();			
			m_ChildDirty = false;
		}
		return m_AllAttr;
	}

	void DataModel::AddModel(DataModel *model)
	{
		assert(model != NULL);
		model->SetParent(model);
		m_Children.push_back(model);
		model->OnAddToModel(this);
		SetChildDirty();
	}

	DataModel* DataModel::GetModel(int model_id, bool only_top)
	{
		//判断是不是自己
		if (m_ModelID == model_id)
		{
			return this;
		}
		
		if (only_top)
		{
			//只搜索直接子节点
			for (DataModelVector::iterator itr = m_Children.begin(); itr != m_Children.end(); ++itr)
			{
				if ((*itr)->GetModelID() == model_id)
				{
					return *itr;
				}
			}
		}
		else
		{
			//递归深度优先搜索
			for (DataModelVector::iterator itr = m_Children.begin(); itr != m_Children.end(); ++itr)
			{
				DataModel *model = (*itr)->GetModel(model_id, false);
				if (model != NULL)
				{
					return model;
				}
			}
		}
		return NULL;
	}

	DataModel* DataModel::GetRoot()
	{
		if (m_Parent == NULL)
		{
			return this;
		}

		DataModel* parent = m_Parent;
		while (parent->GetParent() != NULL)
		{
			parent = parent->GetParent();
		}
		return parent;
	}

	void DataModel::OnBuildAllAttr()
	{
		m_AllAttr.Clear();
		m_AllAttr.AddAttr(GetSelfAttr());
		for (DataModelVector::const_iterator itr = m_Children.cbegin(); itr != m_Children.cend(); ++itr)
		{
			m_AllAttr.AddAttr((*itr)->GetAllAttr());
		}
	}
}