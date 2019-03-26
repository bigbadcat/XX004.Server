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
	//启动设置信息
	class StartSettingInfo
	{
	public:
		StartSettingInfo();
		virtual ~StartSettingInfo();

		//初始化
		virtual void Init(tinyxml2::XMLNode *node);

		//获取类型
		inline int GetType()const { return m_Type; }

		//获取名称
		inline const string& GetName()const { return m_Name; }

		//获取地址
		inline const string& GetIPAddress()const { return m_IPAddress; }

		//获取端口
		inline int GetPort()const { return m_Port; }

		//获取节点值
		static string GetValue(tinyxml2::XMLNode *node, const string &key);

	private:
		int m_Type;
		string m_Name;
		string m_IPAddress;
		int m_Port;
	};

	//启动设置
	class StartSetting
	{
		typedef map<int, StartSettingInfo*> SettingInfoMap;
	public:
		//获取配置
		static StartSetting* GetInstance();

		//获取服务器分组
		inline int GetGroup()const { return m_Group; }

		//获取服务器id
		inline int GetID()const { return m_ID; }

		//获取配置信息
		StartSettingInfo* GetSettingInfo(int type)const;

	private:
		StartSetting();
		virtual ~StartSetting();

		//初始化
		void Init();

		//服务器组
		int m_Group;

		//服务器编号
		int m_ID;

		SettingInfoMap m_Infos;
	};
}

#endif	//__StartSetting_h__
