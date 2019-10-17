/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: StartSetting.h
* Summary: 服务启动设置。
*
* Author: XuXiang
* Date: 2018-12-06 10:42
*******************************************************/

#ifndef __StartSetting_h__
#define __StartSetting_h__

#include "tinyxml2/tinyxml2.h"
#include <string>
#include <map>
using namespace std;

namespace XX004
{
	class MySQLWrap;

	//数据库信息
	class DataBaseSetting
	{
	public:
		DataBaseSetting();
		~DataBaseSetting();

		//初始化
		void Init(tinyxml2::XMLNode *node);

		//数据库地址
		inline const string& GetHost()const { return m_Host; }

		//数据库地址
		inline const string& GetUser()const { return m_User; }

		//数据库地址
		inline const string& GetPassword()const { return m_Password; }

		//数据库地址
		inline const string& GetName()const { return m_Name; }

		//连接端口
		inline int GetPort()const { return m_Port; }

	private:

		//数据库地址
		string m_Host;

		//用户名
		string m_User;

		//密码
		string m_Password;

		//数据库名
		string m_Name;

		//连接端口
		unsigned int m_Port;
	};

	//服务器信息
	class ServerSetting
	{
	public:
		ServerSetting(int t, const char* ip, int port, int http_port);
		~ServerSetting();

		//获取类型
		inline int GetType()const { return m_Type; }

		//获取地址
		inline const string& GetIPAddress()const { return m_IPAddress; }

		//获取端口
		inline int GetPort()const { return m_Port; }

		//获取Http服务端口
		inline int GetHttpPort()const { return m_HttpPort; }

	private:

		//服务器类型
		int m_Type;

		//地址
		string m_IPAddress;

		//端口
		int m_Port;

		//Http服务器端口
		int m_HttpPort;
	};

	//启动设置
	class StartSetting
	{
		typedef map<int, ServerSetting*> ServerSettingMap;
	public:
		//获取配置
		static StartSetting* GetInstance();

		//获取数据库配置
		inline const DataBaseSetting* GetDataBase()const { return &m_Database; }

		//加载服务器信息
		void LoadServerInfo(int type, int sid, MySQLWrap *mysql);

		//获取服务器信息
		ServerSetting* GetServerSetting(int type)const;

		//获取中控端口
		inline short GetMasterPort()const { return m_MasterPort; }

	private:
		StartSetting();
		~StartSetting();

		//初始化
		void Init();

		//数据库配置信息
		DataBaseSetting m_Database;

		//服务器信息
		ServerSettingMap m_ServerSettings;

		//中控服务端口
		short m_MasterPort;
	};
}

#endif
