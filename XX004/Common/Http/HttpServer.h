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
#include <map>
#include <thread>
#include "../Macro.h"
using namespace std;

struct evhttp_request;
struct evbuffer;

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

	private:
		struct evbuffer *m_ResBuff;
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

		//每次循环调用
		virtual void OnLoop() = 0;

		//释放
		virtual void OnRelease() = 0;

		//注册服务
		void RegisterServer(string path, HttpServerCallBack call);

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

		HttpServerCallBackMap m_ServerCallBack;
	};
}

#endif