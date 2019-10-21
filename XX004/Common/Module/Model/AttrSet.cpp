/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: AttrSet.cpp
* Summary: 属性集。
*
* Author: XuXiang
* Date: 2019-10-21 16:17
*******************************************************/

#include "AttrSet.h"

namespace XX004
{
	AttrSet::AttrSet()
	{
	}

	AttrSet::~AttrSet()
	{
	}

	void AttrSet::AddAttr(const map<int, Int64>& attrs)
	{
		for (AttrMap::const_iterator itr = attrs.cbegin(); itr != attrs.cend(); ++itr)
		{
			m_Attrs[itr->first] = m_Attrs[itr->first] + itr->second;
		}
	}
}