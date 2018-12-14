/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MainBase.cpp
* Summary: 程序入口管理的基类。
*
* Author: XuXiang
* Date: 2018-08-13 22:43
*******************************************************/

#include "MainBase.h"
#include "NetManagerBase.h"
#include "ServerBase.h"
#include "StartSetting.h"
#include <iostream>
#include <assert.h>
#include <lua.hpp>

namespace XX004
{
	MainBase* MainBase::pCurMain = NULL;

	const Int64 MainBase::FRAME_GAP = 100;

	const string MainBase::COMMAND_QUIT = "/q";

	MainBase::MainBase() : m_Type(0), m_pNetManager(NULL), m_pServer(NULL)
	{
		pCurMain = this;
	}

	MainBase::~MainBase()
	{
		if (pCurMain == this)
		{
			pCurMain = NULL;
		}
		SAFE_DELETE(m_pNetManager);
		SAFE_DELETE(m_pServer);
	}

	int MainBase::Run(int type)
	{
		//初始化网络
		WORD wVersionRequested;
		WSADATA wsaData;
		int32_t err;
		wVersionRequested = MAKEWORD(2, 2);
		err = ::WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			cout << "WSAStartup err:" << err << endl;
			return 1;
		}

		//测试Lua
		lua_State *L = luaL_newstate();
		luaL_openlibs(L);
		err = luaL_dofile(L, "LuaTest.lua.txt");
		if (err != LUA_OK)
		{
			const char *msg = lua_tostring(L, -1);
			if (msg != NULL)
			{
				cout << "Lua error!\n" << lua_tostring(L, -1) << endl;
			}			
		}

		//读取表格
		if (lua_istable(L, -1))
		{
			lua_pushnil(L);
			while (lua_next(L, -2))			//堆栈状态 -1:value(table) -2:key -3:table
			{
				//读取属性1
				lua_pushnumber(L, 1);
				lua_gettable(L, -2);
				int id = lua_tonumber(L, -1);
				lua_pop(L, 1);

				//读取属性2
				lua_pushnumber(L, 2);
				lua_gettable(L, -2);
				string name = string(lua_tostring(L, -1));
				lua_pop(L, 1);

				//读取属性3
				lua_pushnumber(L, 3);
				lua_gettable(L, -2);
				int level = lua_tonumber(L, -1);
				lua_pop(L, 1);
				
				int key = (int)lua_tonumber(L, -2);
				cout << "key:" << key << " value:{id:" << id << ", name:" << name << ", level:" << level << "}" << endl;
				lua_pop(L, 1);		//把值弹出去
			}
		}
		else
		{
			cout << "Lua file is not return a table." << endl;
		}

		lua_close(L);
		L = NULL;

		//模块创建		
		m_Type = type;
		m_pNetManager = OnCreateNetManager();
		assert(m_pNetManager != NULL);
		m_pServer = OnCreateServer();
		assert(m_pServer != NULL);
		m_pServer->RegisterNetMessage(m_pNetManager);

		//模块运行
		StartSettingInfo* info = StartSetting::GetInstance()->GetSettingInfo(m_Type);
		assert(info != NULL);
		m_pNetManager->Start(info->GetPort());
		m_pServer->Start(true);
		CommandLoop();
		cout << "Waitting server end ..." << endl;
		m_pServer->Stop();		//Server停止还需要依赖网络，如逻辑服停止后需要把服务器状态数据提交给数据服
		m_pServer->Join();
		cout << "Waitting net end ..." << endl;
		m_pNetManager->Stop();
		m_pNetManager->Join();

		//模块销毁
		SAFE_DELETE(m_pNetManager);
		SAFE_DELETE(m_pServer);
		::WSACleanup();

		return 0;
	}

	void MainBase::CommandLoop()
	{
		cout << "Start command ..." << endl;
		char str[64];
		while (true)
		{
			//接收命令
			cin.getline(str, sizeof(str));
			
			//如果输入内容超过缓冲区
			if (!std::cin)
			{
				std::cin.clear(); // 清除错误标志位
				std::cin.sync(); // 清除流
			}

			//提交命令
			string cmd(str);
			if (cmd.compare(COMMAND_QUIT) == 0)
			{
				break;
			}
			else
			{
				m_pServer->PostCommand(cmd);
			}			
		}
	}
}