/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: HttpServer.cpp
* Summary: Http服务基类。
*
* Author: XuXiang
* Date: 2019-09-27 17:07
*******************************************************/

#include "HttpServer.h"
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>
#include "../Util/StringUtil.h"

namespace XX004
{
	HttpResponse::HttpResponse() : m_ResBuff(NULL)
	{
		m_ResBuff = evbuffer_new();
	}

	HttpResponse::~HttpResponse()
	{
		if (m_ResBuff != NULL)
		{
			evbuffer_free(m_ResBuff);
			m_ResBuff = NULL;
		}
	}

	void HttpResponse::AddText(const char *fmt, ...)
	{
		va_list ap;
		va_start(ap, fmt);	//获得可变参数列表,分析fmt，得出各个参数的类型
		evbuffer_add_vprintf(m_ResBuff, fmt, ap);
		va_end(ap);			//释放资源,必须与va_start成对调用
	}

	void HttpResponse::SendReply(struct evhttp_request *req)
	{
		evhttp_send_reply(req, HTTP_OK, NULL, m_ResBuff);
	}

	HttpServer::HttpServer() : m_Run(false), m_Port(0)
	{		
	}

	HttpServer::~HttpServer()
	{		
	}

	void HttpServer::Start(short port)
	{
		m_Port = port;
		m_Run = true;
		m_Thread = thread([](HttpServer *t){t->ThreadProcess(); }, this);
	}

	void HttpServer::Stop()
	{
		m_Run = false;
		JoinThread(m_Thread);
	}

	string HttpServer::GetHttpParams(const string &uri, HttpParamMap &params)
	{
		//通过?确定路径和参数部分
		size_t pos = uri.find_first_of('?');
		params.clear();
		if (pos == string::npos)
		{
			return uri;
		}
		string path = uri.substr(0, pos);
		string p_str = uri.substr(pos + 1);
	
		//解析参数
		vector<string> v;
		StringUtil::Split(p_str, "&", v);
		for (vector<string>::iterator itr = v.begin(); itr != v.end(); ++itr)
		{
			size_t p = itr->find_first_of('=');
			string key = itr->substr(0, p);
			string value = itr->substr(p+1);
			params[key] = value;
		}
		return path;
	}

	void HttpServer::RegisterServer(string path, HttpServerCallBack call)
	{
		m_ServerCallBack[path] = call;
	}

	void HttpServer::ThreadProcess()
	{
		struct event_base *base = event_base_new();
		printf("Using Libevent with backend method <%s>.\n", event_base_get_method(base));	//获取当前使用了哪种I/O模型，Windows下仅显示win32
		
		//启动服务
		char *http_addr = "0.0.0.0";
		struct evhttp *http_server = evhttp_new(base);
		evhttp_bind_socket(http_server, http_addr, m_Port);
		evhttp_set_gencb(http_server, HttpServer::OnHttpRequest, this);
		OnRegisterServer();
		
		//处理请求1秒钟最多处理10个
		OnInit();
		while (m_Run)
		{
			event_base_loop(base, EVLOOP_NONBLOCK);
			OnLoop();
			chrono::milliseconds dura(100);
			this_thread::sleep_for(dura);
		}
		OnRelease();
		
		evhttp_free(http_server);
		http_server = NULL;		
		event_base_free(base);
		base = NULL;
	}

	void HttpServer::OnHttpRequest(struct evhttp_request *req, void *arg)
	{
		//解析参数
		XX004::HttpServer *pserver = (XX004::HttpServer*)arg;
		string uri(evhttp_request_get_uri(req));
		HttpParamMap params_set;
		string path = GetHttpParams(uri, params_set);

		//分派服务
		HttpServerCallBackMap::iterator itr = pserver->m_ServerCallBack.find(path);
		if (itr == pserver->m_ServerCallBack.end())
		{
			struct evbuffer *retbuff = evbuffer_new();
			evbuffer_add_printf(retbuff, "The requested URL %s was not found on this server.", uri.c_str());
			evhttp_send_reply(req, HTTP_NOTFOUND, "Not Found", retbuff);
			evbuffer_free(retbuff);
			retbuff = NULL;
			return;
		}

		//处理服务，回复结果
		HttpResponse res;
		itr->second(path, params_set, res);
		res.SendReply(req);
	}
}