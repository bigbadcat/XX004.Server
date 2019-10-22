/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: AttrSet.h
* Summary: 属性集。
*
* Author: XuXiang
* Date: 2019-10-21 16:17
*******************************************************/

#ifndef __AttrSet_h__
#define __AttrSet_h__

#include "../../Macro.h"
#include <map>
using namespace std;

namespace XX004
{
	typedef map<int, Int64> AttrMap;
	class AttrSet
	{
	public:
		AttrSet();
		~AttrSet();

		//清除属性
		inline void Clear() { m_Attrs.clear(); }

		//设置属性
		inline void SetAttr(int type, Int64 value) { m_Attrs[type] = value; }

		//获取属性
		inline Int64 GetAttr(int type)const;

		//添加属性
		inline void AddAttr(int type, Int64 value) { SetAttr(type, GetAttr(type) + value); }

		//添加属性
		void AddAttr(const map<int, Int64>& attrs);

		//添加属性
		inline void AddAttr(const AttrSet& attr_set) { AddAttr(attr_set.m_Attrs); }

		//获取所有属性
		const AttrMap& GetAllAtrr()const { return m_Attrs; }

	private:

		//属性内容
		AttrMap m_Attrs;
	};

	inline Int64 AttrSet::GetAttr(int type)const
	{
		AttrMap::const_iterator itr = m_Attrs.find(type);
		return itr == m_Attrs.cend() ? 0 : itr->second;
	}
}

#endif