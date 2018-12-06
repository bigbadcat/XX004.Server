/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: StartSetting.cpp
* Summary: 服务启动设置。
*
* Author: XuXiang
* Date: 2018-12-06 10:42
*******************************************************/

#include "StartSetting.h"
#include "Macro.h"
#include <assert.h>
#include <iostream>

namespace XX004
{
	StartSettingInfo::StartSettingInfo() : m_Type(0), m_Port(0)
	{

	}

	StartSettingInfo::~StartSettingInfo()
	{

	}

	void StartSettingInfo::Init(tinyxml2::XMLNode *node)
	{
		m_Type = ::atoi(GetValue(node, "type").c_str());
		m_Name = GetValue(node, "name");
		m_IPAddress = GetValue(node, "ip");
		m_Port = ::atoi(GetValue(node, "port").c_str());
	}

	string StartSettingInfo::GetValue(tinyxml2::XMLNode *node, const string &key)
	{
		assert(node != NULL);
		tinyxml2::XMLElement *key_child = node->FirstChildElement(key.c_str());
		if (key_child != NULL)
		{
			tinyxml2::XMLNode *text_child = key_child->FirstChild();
			if (text_child != NULL)
			{
				tinyxml2::XMLText *text_node = text_child->ToText();
				if (text_node != NULL)
				{
					return string(text_node->Value());
				}
			}			
		}
		return string();
	}

	StartSetting* StartSetting::GetInstance()
	{
		static StartSetting instance;
		static bool init = false;
		if (!init)
		{
			instance.Init();
			init = true;
		}
		return &instance;
	}

	StartSettingInfo* StartSetting::GetSettingInfo(int type)const
	{
		SettingInfoMap::const_iterator citr = m_Infos.find(type);
		return citr == m_Infos.cend() ? NULL : citr->second;
	}

	StartSetting::StartSetting()
	{

	}

	StartSetting::~StartSetting()
	{
		for (SettingInfoMap::iterator itr = m_Infos.begin(); itr != m_Infos.end(); ++itr)
		{
			SAFE_DELETE(itr->second);
		}
		m_Infos.clear();
	}

	void StartSetting::Init()
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.LoadFile("StartSetting.xml");
		assert(error == tinyxml2::XMLError::XML_SUCCESS);
		tinyxml2::XMLElement *root = doc.FirstChildElement("StartSetting");	
		for (tinyxml2::XMLElement *itr = root->FirstChildElement(); itr != NULL; itr = itr->NextSiblingElement())
		{
			//cout << "type:" << itr->FirstChildElement("type")->FirstChild()->ToText()->Value() << endl;
			StartSettingInfo *info = new StartSettingInfo();
			info->Init(itr);
			m_Infos.insert(SettingInfoMap::value_type(info->GetType(), info));
		}
	}
}