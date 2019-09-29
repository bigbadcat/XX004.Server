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

#include <Http/HttpServer.h>
#include <string>
using namespace std;

namespace XX004
{
	class ServerMaster : public HttpServer
	{
	public:
		ServerMaster();
		virtual ~ServerMaster();

	protected:
		virtual void OnRegisterServer();

		//初始化
		virtual void OnInit();

		//每次循环调用
		virtual void OnLoop();

		//释放
		virtual void OnRelease();

	private:

		//登陆请求
		void OnLoginRequest(const string &path, HttpParamMap &params, HttpResponse &res);
	};
}

#endif