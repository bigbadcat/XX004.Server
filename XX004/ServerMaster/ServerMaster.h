/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerMaster.h
* Summary: 中控服务。
*
* Author: XuXiang
* Date: 2019-09-27 17:25
*******************************************************/

#ifndef __ServerMaster_h__
#define __ServerMaster_h__

#include <Macro.h>
#include <Http/HttpServer.h>
#include <string>
#include <vector>
using namespace std;

namespace XX004
{
	class MySQLResult;
	class MySQLWrap;

	//服务器信息
	struct ServerInfo
	{
		ServerInfo();
		~ServerInfo();

		void Init(MySQLResult *result);

		int id;
		int group;
		int index;
		string ip;
		int port;
		int http_port;
		int type;
		int state;
		int recommend;
		Int64 open_time;
	};

	typedef vector<ServerInfo*> ServerInfoVector;

	//服务器组信息
	struct ServerGroupInfo
	{
		ServerGroupInfo();
		~ServerGroupInfo();

		//初始化服务器组
		void Init(MySQLResult *result);

		//添加服务器信息
		void AddServerInfo(ServerInfo *info);

		//获取服务器信息
		const ServerInfo* GetServerInfo(int sid);

		int id;
		vector<int> sub_channel;
		ServerInfoVector servers;
	};

	typedef map<int, ServerGroupInfo*> ServerGroupInfoMap;

	class ServerMaster : public HttpServer
	{
	public:
		ServerMaster();
		virtual ~ServerMaster();

		//设置服务器数据需要刷新
		void SetServerDirty();

		//服务器信息失效间隔
		static const UInt64 SERVER_DIRTY_GAP;

	protected:
		virtual void OnRegisterServer();

		//初始化
		virtual void OnInit();

		//释放
		virtual void OnRelease();

	private:

		//获取服务器信息
		const ServerInfo* GetServerInfo(int sid)const;

		//转发Http请求到游戏服
		void RelayHttpRequest(const string &path, HttpParamMap &params, HttpResponse &res);

		//加载服务器信息
		void LoadServerInfo();

		//检查服务器信息
		void CheckServerInfo();

		//登陆请求
		void OnLoginRequest(const string &path, HttpParamMap &params, HttpResponse &res);
		bool CheckAccount(const string& user, const string& password, Int64 &create_time, HttpResponse &res);
		void ReplyLogin(const string& user, Int64 create_time, int sub_channel, HttpResponse &res);

		//MySQL接口
		MySQLWrap *m_MySQL;

		//服务器数据是否需要刷新
		bool m_IsServerDirty;

		//服务器数据加载时间
		UInt64 m_ServerTimeStamp;

		//服务器组信息
		ServerGroupInfoMap m_ServerGroups;
	};
}

#endif