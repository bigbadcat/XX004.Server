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
#include "MySQL/MySQLWrap.h"
#include "Net/NetDefine.h"
#include <assert.h>
#include <iostream>

namespace XX004
{
	//获取XML值
	string get_xml_value(tinyxml2::XMLNode *node, const string &key)
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

	DataBaseSetting::DataBaseSetting() : m_Port(0)
	{

	}

	DataBaseSetting::~DataBaseSetting()
	{

	}

	void DataBaseSetting::Init(tinyxml2::XMLNode *node)
	{
		m_Host = get_xml_value(node, "host");
		m_User = get_xml_value(node, "user");
		m_Password = get_xml_value(node, "pwd");
		m_Name = get_xml_value(node, "name");
		m_Port = (unsigned int)::atoi(get_xml_value(node, "port").c_str());
	}

	ServerSetting::ServerSetting(int t, const char* ip, int port, int http_port) : m_Type(t), m_IPAddress(ip), m_Port(port), m_HttpPort(http_port)
	{
	}

	ServerSetting::~ServerSetting()
	{		
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

	//读取游戏服设置
	ServerSetting* load_game_setting(int sid, int *rid, MySQLWrap *mysql)
	{
		//查询服务器
		ServerSetting *setting = NULL;
		char sql[64];
		sprintf_s(sql, "SELECT * FROM tb_server WHERE id = %d", sid);
		auto ret = mysql->Query(sql);
		if (ret->GetRecord())
		{
			string ip = ret->GetString("ip");
			int port = ret->GetInt("port");
			int http_port = ret->GetInt("http_port");
			*rid = ret->GetInt("region");
			setting = new ServerSetting(XX004::Net::RemoteType::RT_GAME, ip.c_str(), port, http_port);
		}
		ret->Clear();

		return setting;
	}

	//读取区域服设置
	ServerSetting* load_region_setting(int sid, MySQLWrap *mysql)
	{
		//查询服务器
		ServerSetting *setting = NULL;
		char sql[64];
		sprintf_s(sql, "SELECT * FROM tb_server_region WHERE id = %d", sid);
		auto ret = mysql->Query(sql);
		if (ret->GetRecord())
		{
			string ip = ret->GetString("ip");
			int port = ret->GetInt("port");
			int http_port = ret->GetInt("http_port");
			setting = new ServerSetting(XX004::Net::RemoteType::RT_REGION, ip.c_str(), port, http_port);
		}
		ret->Clear();
		return setting;
	}

	void StartSetting::LoadServerInfo(int type, int sid, MySQLWrap *mysql)
	{
		if (type == XX004::Net::RemoteType::RT_GAME)
		{
			int rid = 0;
			m_ServerSettings[XX004::Net::RemoteType::RT_GAME] = load_game_setting(sid, &rid, mysql);
			m_ServerSettings[XX004::Net::RemoteType::RT_REGION] = load_region_setting(rid, mysql);
		}
		else if (type == XX004::Net::RemoteType::RT_REGION)
		{
			m_ServerSettings[XX004::Net::RemoteType::RT_REGION] = load_region_setting(sid, mysql);
		}
	}

	ServerSetting* StartSetting::GetServerSetting(int type)const
	{
		ServerSettingMap::const_iterator citr = m_ServerSettings.find(type);
		return citr == m_ServerSettings.cend() ? NULL : citr->second;
	}

	StartSetting::StartSetting()
	{
	}

	StartSetting::~StartSetting()
	{
		SAFE_DELETE_MAP(m_ServerSettings);
	}

	void StartSetting::Init()
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.LoadFile("StartSetting.xml");
		assert(error == tinyxml2::XMLError::XML_SUCCESS);
		tinyxml2::XMLElement *root = doc.FirstChildElement("StartSetting");
		for (tinyxml2::XMLElement *itr = root->FirstChildElement(); itr != NULL; itr = itr->NextSiblingElement())
		{
			tinyxml2::XMLNode *node = itr;
			string name(node->Value());
			if (name.compare("DataBase") == 0)
			{
				m_Database.Init(node);
			}
		}
	}
}