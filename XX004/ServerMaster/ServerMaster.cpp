/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerMaster.cpp
* Summary: 中控服务。
*
* Author: XuXiang
* Date: 2019-09-27 17:25
*******************************************************/

#include "ServerMaster.h"
#include <MySQL/MySQLWrap.h>
#include <Frame/StartSetting.h>
#include <Util/TimeUtil.h>
#include <Util/StringUtil.h>
#include <iostream>
#include <regex>
using namespace std;

namespace XX004
{
	ServerInfo::ServerInfo() : id(0), group(0), index(0), port(0), http_port(0), type(0), state(0), recommend(0), open_time(0)
	{

	}

	ServerInfo::~ServerInfo()
	{

	}

	void ServerInfo::Init(MySQLResult *result)
	{
		id = result->GetInt("id");
		group = result->GetInt("group");
		index = result->GetInt("index");
		ip = result->GetString("ip");
		port = result->GetInt("port");
		http_port = result->GetInt("http_port");
		type = result->GetInt("type");
		state = result->GetInt("state");
		recommend = result->GetInt("recommend");
		open_time = result->GetInt64("open_time");
	}

	ServerGroupInfo::ServerGroupInfo() : id(0)
	{

	}

	ServerGroupInfo::~ServerGroupInfo()
	{
		SAFE_DELETE_VECTOR(servers);
	}

	void ServerGroupInfo::Init(MySQLResult *result)
	{
		id = result->GetInt("id");
		StringUtil::SplitToInt(result->GetString("sub_channel"), ",", sub_channel);

		if (sub_channel.size() == 0)
		{
			printf_s("ServerGroup(id:%d) not set sub_channel.\n", id);
		}
	}

	void ServerGroupInfo::AddServerInfo(ServerInfo *info)
	{
		servers.push_back(info);
	}

	const ServerInfo* ServerGroupInfo::GetServerInfo(int sid)
	{
		for (ServerInfoVector::const_iterator itr = servers.cbegin(); itr != servers.cend(); ++itr)
		{
			if ((*itr)->id == sid)
			{
				return *itr;
			}
		}
		return NULL;
	}

	const UInt64 ServerMaster::SERVER_DIRTY_GAP = 60 * 5;		//5分钟失效

	ServerMaster::ServerMaster() : m_MySQL(new MySQLWrap()), m_IsServerDirty(false), m_ServerTimeStamp(0)
	{		
	}

	ServerMaster::~ServerMaster()
	{
		SAFE_DELETE(m_MySQL);
		SAFE_DELETE_MAP(m_ServerGroups);
	}

	void ServerMaster::SetServerDirty()
	{
		::printf_s("ServerMaster::SetServerDirty\n");
		m_IsServerDirty = true;		
	}

	void ServerMaster::OnRegisterServer()
	{
		RegisterDefaultServer([this](const string &p, HttpParamMap &params, HttpResponse &res){this->RelayHttpRequest(p, params, res); });
		HTTP_SERVER_REGISTER("/login", OnLoginRequest);
	}

	void ServerMaster::OnInit()
	{
		const DataBaseSetting* db = StartSetting::GetInstance()->GetDataBase();
		m_MySQL->Init(db->GetHost().c_str(), db->GetUser().c_str(), db->GetPassword().c_str(), db->GetName().c_str(), db->GetPort());
		LoadServerInfo();
	}

	void ServerMaster::OnRelease()
	{
		m_MySQL->Release();
	}

	const ServerInfo* ServerMaster::GetServerInfo(int sid)const
	{
		int gid = sid / 10000;
		ServerGroupInfoMap::const_iterator itr = m_ServerGroups.find(gid);
		if (itr == m_ServerGroups.cend())
		{
			return NULL;
		}
		return itr->second->GetServerInfo(sid);
	}

	void ServerMaster::RelayHttpRequest(const string &path, HttpParamMap &params, HttpResponse &res)
	{
		HttpParamMap::iterator itr = params.find("sid");
		if (itr == params.end())
		{
			res.AddText("RelayHttpRequest need \"sid\" param.");
			return;
		}

		int sid = ::atoi(itr->second.c_str());
		if (sid <= 0)
		{
			res.AddText("RelayHttpRequest bad sid(%s).", itr->second.c_str());
			return;
		}

		const ServerInfo *info = GetServerInfo(sid);
		if (info == NULL)
		{
			res.AddText("RelayHttpRequest server(%d) not found.", sid);
			return;
		}

		//回复并转发请求
		HttpParamMap new_param(params);
		new_param.erase("sid");		
		SendHttpRequest(info->ip, info->http_port, path, new_param);
		res.AddText("Relay to %d. Time:%I64d", sid, TimeUtil::GetCurrentSecond());
	}

	void ServerMaster::LoadServerInfo()
	{
		m_ServerTimeStamp = TimeUtil::GetCurrentSecond();

		//读取服务器组
		char sql[64];
		sprintf_s(sql, "SELECT * FROM tb_server_group");
		auto ret = m_MySQL->Query(sql);
		while (ret->GetRecord())
		{
			ServerGroupInfo *info = new ServerGroupInfo();
			info->Init(ret.get());
			m_ServerGroups.insert(make_pair(info->id, info));
		}
		ret->Clear();

		//读取服务器
		sprintf_s(sql, "SELECT * FROM tb_server");
		ret = m_MySQL->Query(sql);
		while (ret->GetRecord())
		{
			ServerInfo *info = new ServerInfo();
			info->Init(ret.get());
			ServerGroupInfoMap::iterator itr = m_ServerGroups.find(info->group);
			if (itr != m_ServerGroups.end())
			{
				itr->second->AddServerInfo(info);
			}
			else
			{
				printf_s("Unkown server group id:%d.\n", info->group);
				SAFE_DELETE(info);
			}
		}
	}

	void ServerMaster::CheckServerInfo()
	{
		if (m_IsServerDirty || TimeUtil::GetCurrentSecond() > (m_ServerTimeStamp + SERVER_DIRTY_GAP))
		{
			m_IsServerDirty = false;
			SAFE_DELETE_MAP(m_ServerGroups);
			LoadServerInfo();
		}
	}

	void ServerMaster::OnLoginRequest(const string &path, HttpParamMap &params, HttpResponse &res)
	{
		string user = params["user"];
		string pwd = params["pwd"];
		string sub_channel_str = params["sub_channel"];
		int sub_channel = sub_channel_str.empty() ? 0 : ::atoi(sub_channel_str.c_str());
		printf_s("OnLoginRequest cmd:%s user:%s pwd:%s sub_channel:%d\n", path.c_str(), user.c_str(), pwd.c_str(), sub_channel);

		Int64 create_time = 0;
		if (CheckAccount(user, pwd, create_time, res))
		{
			ReplyLogin(user, create_time, sub_channel, res);
		}
	}

	bool ServerMaster::CheckAccount(const string& user, const string& pwd, Int64 &create_time, HttpResponse &res)
	{
		//result 0:成功 1:密码错误 2:账号非法 3:账号冻结
		regex reg("^[a-zA-Z0-9_]*$");  //正则匹配格式
		bool ok = user.length() >= 4 && user.length() <= 16 && regex_match(user, reg);
		if (!ok)
		{
			res.BeginLuaTable();
			res.AddLuaKeyValue("Result", 2);
			res.AddLuaKeyValue("User", user.c_str());
			res.EndLuaTable();
			return false;
		}

		//查询数据库
		char sql[64];
		sprintf_s(sql, "call sp_select_user('%s');", user.c_str());
		auto ret = m_MySQL->Query(sql);
		if (ret->GetRecord())
		{
			//读取账号信息
			string password = ret->GetString("password");
			if (password.compare(pwd) != 0)
			{
				res.BeginLuaTable();
				res.AddLuaKeyValue("Result", 1);
				res.AddLuaKeyValue("User", user.c_str());
				res.EndLuaTable();
				return false;
			}
			create_time = ret->GetInt64("create_time");
			Int64 freetime = ret->GetInt64("free_time");
			if (freetime > (Int64)TimeUtil::GetCurrentSecond())
			{
				res.BeginLuaTable();
				res.AddLuaKeyValue("Result", 3);
				res.AddLuaKeyValue("User", user.c_str());
				res.AddLuaKeyValue("FreeTime", freetime);
				res.EndLuaTable();
				return false;
			}
		}
		else
		{
			//创建账号
			create_time = TimeUtil::GetCurrentSecond();

			char sql[128];
			sprintf_s(sql, "call sp_insert_update_user('%s','%s',%I64d,%I64d);", user.c_str(), pwd.c_str(), create_time, (Int64)0);
			m_MySQL->Execute(sql);
		}
		return true;
	}

	void ServerMaster::ReplyLogin(const string& user, Int64 create_time, int sub_channel, HttpResponse &res)
	{
		//回复结果		
		char token[32];
		sprintf_s(token, sizeof(token), "%s&%I64d", user.c_str(), TimeUtil::GetCurrentSecond());
		res.BeginLuaTable();
		res.AddLuaKeyValue("Result", 0);
		res.AddLuaKeyValue("User", user.c_str());
		res.AddLuaKeyValue("FreeTime", 0);
		res.AddLuaKeyValue("Token", token);

		//服务器列表 table.insert(servers, {id=1, list={{id=10001, state=1, ip="192.168.0.104", port=9000}}})
		int group_count = 0;
		CheckServerInfo();
		res.AddLuaKey("Servers");
		res.BeginLuaTable();
		for (ServerGroupInfoMap::iterator itr = m_ServerGroups.begin(); itr != m_ServerGroups.end(); ++itr)
		{
			ServerGroupInfo *ginfo = itr->second;
			if (find(ginfo->sub_channel.begin(), ginfo->sub_channel.end(), sub_channel) != ginfo->sub_channel.end())
			{
				res.AddLuaKey(NULL);
				res.BeginLuaTable();
				res.AddLuaKeyValue("id", ginfo->id);
				res.AddLuaKey("list");
				res.BeginLuaTable();
				int server_count = 0;
				for (ServerInfoVector::iterator iitr = ginfo->servers.begin(); iitr != ginfo->servers.end(); ++iitr)
				{
					ServerInfo *sinfo = *iitr;
					res.AddLuaKey(NULL);
					res.BeginLuaTable();
					res.AddLuaKeyValue("id", sinfo->id);
					res.AddLuaKeyValue("index", sinfo->index);
					res.AddLuaKeyValue("ip", sinfo->ip.c_str());
					res.AddLuaKeyValue("port", sinfo->port);
					res.AddLuaKeyValue("type", sinfo->type);
					res.AddLuaKeyValue("state", sinfo->state);
					res.AddLuaKeyValue("recommend", sinfo->recommend);
					res.AddLuaKeyValue("open_time", sinfo->open_time);
					res.EndLuaTable();
				}
				res.EndLuaTable();
				res.EndLuaTable();
			}
		}
		res.EndLuaTable();
		res.EndLuaTable();
	}
}