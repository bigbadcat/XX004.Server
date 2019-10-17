/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: HttpServer.h
* Summary: Http服务基类。
*
* Author: XuXiang
* Date: 2019-09-27 17:07
*******************************************************/

#ifndef __HttpServer_h__
#define __HttpServer_h__

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <thread>
#include "../Macro.h"
using namespace std;

struct evhttp_request;
struct evbuffer;
struct event_base;

namespace XX004
{
	//Http回复
	class HttpResponse
	{
	public:
		HttpResponse();
		~HttpResponse();

		//添加文本
		void AddText(const char *fmt, ...);

		//发送回复
		void SendReply(struct evhttp_request *req);

		void AddLuaKey(const char *key);		//key为NULL表示添加数组元素
		void BeginLuaTable();
		void EndLuaTable();
		void AddLuaKeyValue(const char *key, int value);
		void AddLuaKeyValue(const char *key, Int64 value);
		void AddLuaKeyValue(const char *key, const char *value);

	private:

		//添加Lua字段计数 ret:添加计数前的值
		int AddLuaKeyCount();

		//回复缓冲区
		struct evbuffer *m_ResBuff;

		//LuaTabe添加key的数量
		stack<int> m_LuaTableKeyCount;
	};

	typedef map<string, string> HttpParamMap;
	typedef std::function<void(const string&, HttpParamMap&, HttpResponse&)> HttpServerCallBack;
	typedef std::map<string, HttpServerCallBack> HttpServerCallBackMap;

#define HTTP_SERVER_REGISTER(path, fun) \
	RegisterServer(path, [this](const string &p, HttpParamMap &params, HttpResponse &res){this->fun(p, params, res); })

	class HttpServer
	{
	public:
		HttpServer();
		virtual ~HttpServer();

		//启动服务 port:服务端口
		void Start(short port);

		//停止服务
		void Stop();

		static string GetHttpParams(const string &uri, HttpParamMap &params);

	protected:
		//开始注册服务
		virtual void OnRegisterServer() = 0;

		//初始化
		virtual void OnInit() = 0;

		//释放
		virtual void OnRelease() = 0;

		//注册服务
		void RegisterServer(string path, HttpServerCallBack call);

		//设置默认服务
		inline void RegisterDefaultServer(HttpServerCallBack call) { m_DefaultCallBack = call; }

	private:
		//线程过程
		void ThreadProcess();

		//收到Http请求
		static void OnHttpRequest(struct evhttp_request *req, void *arg);

		bool m_Run;

		//逻辑处理线程
		thread m_Thread;
		
		//服务端口
		short m_Port;

		//事件基础模块
		struct event_base *m_EventBase;

		//服务回调
		HttpServerCallBackMap m_ServerCallBack;

		//默认回掉
		HttpServerCallBack m_DefaultCallBack;
	};
}

#endif