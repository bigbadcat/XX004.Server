﻿/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MySQLResult.cpp
* Summary: MySQL查询结果。
*
* Author: XuXiang
* Date: 2019-01-23 15:12
*******************************************************/

#include "MySQLResult.h"
#include <assert.h>

namespace XX004
{
	MySQLResult::MySQLResult() : m_Result(NULL), m_CurRecord(NULL)
	{		
	}

	MySQLResult::~MySQLResult()
	{		
		if (m_Result != NULL)
		{
			mysql_free_result(m_Result);
			m_Result = NULL;
		}
		m_CurRecord = NULL;
	}

	void MySQLResult::Init(MYSQL_RES *res)
	{
		m_FieldNames.clear();
		m_NameToIndex.clear();
		m_Result = res;
		m_CurRecord = NULL;

		//解析字段名
		int num_fields = mysql_num_fields(m_Result);
		assert(num_fields > 0);
		MYSQL_FIELD *fields = mysql_fetch_fields(m_Result);
		assert(fields != NULL);
		for (int i = 0; i < num_fields; ++i)
		{
			m_FieldNames.push_back(fields[i].name);
			m_NameToIndex.insert(NameIndexMap::value_type(m_FieldNames[i], i));
		}
	}

	bool MySQLResult::GetRecord()
	{
		m_CurRecord = mysql_fetch_row(m_Result);
		return m_CurRecord != NULL;
	}

	int MySQLResult::GetIndex(const char *name)
	{
		NameIndexMap::iterator itr = m_NameToIndex.find(name);
		return itr == m_NameToIndex.end() ? -1 : itr->second;
	}

	int MySQLResult::GetInt(int index)
	{
		char *value_str = m_CurRecord[index];
		int i = ::atoi(value_str);
		return i;
	}

	__int64 MySQLResult::GetInt64(int index)
	{
		char *value_str = m_CurRecord[index];
		__int64 i = ::_atoi64(value_str);
		return i;
	}

	float MySQLResult::GetFloat(int index)
	{
		char *value_str = m_CurRecord[index];
		float f = ::atof(value_str);
		return f;
	}

	const char* MySQLResult::GetString(int index)
	{
		return m_CurRecord[index];
	}
}