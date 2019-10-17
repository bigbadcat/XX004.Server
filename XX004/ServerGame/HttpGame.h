/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: HttpGame.h
* Summary: 游戏服HTTP服务。
*
* Author: XuXiang
* Date: 2019-10-17 10:05
*******************************************************/

#ifndef __Http_Game_h__
#define __Http_Game_h__

#include <Macro.h>
#include <Http/HttpServer.h>
#include <Util/LockQueue.h>
#include <string>
#include <vector>
using namespace std;

namespace XX004
{
	//Http请求信息
	struct HttpRequestInfo
	{
		string path;
		HttpParamMap params;
	};

	typedef std::function<void(const string&, HttpParamMap&)> HttpCallBack;
	typedef std::map<string, HttpCallBack> HttpCallBackMap;
	typedef std::queue<HttpRequestInfo*> HttpRequestInfoQueue;
	typedef LockQueue<HttpRequestInfo> HttpRequestInfoLockQueue;

#define HTTP_GAME_REGISTER(path, fun) \
	RegisterServer(path, [this](const string &p, HttpParamMap &params){this->fun(p, params); })

	class HttpGame : public HttpServer
	{
	public:
		HttpGame();
		virtual ~HttpGame();

		//分发请求，此函数由服务逻辑线程调用
		void Dispatch();

	protected:
		virtual void OnRegisterServer();

		//初始化
		virtual void OnInit();

		//释放
		virtual void OnRelease();

	private:

		//注册服务
		void RegisterServer(string path, HttpCallBack call);

		//提交Http请求
		void PostHttpRequest(const string &path, HttpParamMap &params, HttpResponse &res);

		//踢人请求
		void OnKickRequest(const string &path, HttpParamMap &params);

		//请求回调
		HttpCallBackMap m_HttpCallBack;

		//请求队列
		HttpRequestInfoLockQueue m_RequestQueue;
	};
}

#endif