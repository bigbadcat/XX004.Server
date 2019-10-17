/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: HttpGame.cpp
* Summary: 游戏服HTTP服务。
*
* Author: XuXiang
* Date: 2019-10-17 10:05
*******************************************************/

#include "HttpGame.h"
#include <Util/TimeUtil.h>

namespace XX004
{
	HttpGame::HttpGame()
	{		
	}

	HttpGame::~HttpGame()
	{
	}

	void HttpGame::Dispatch()
	{
		//先锁定队列，将消息都拿出来，解锁队列
		static HttpRequestInfoQueue temp_queue;
		m_RequestQueue.MoveAll(temp_queue);

		//分发消息
		while (temp_queue.size() > 0)
		{
			HttpRequestInfo *item = temp_queue.front();
			temp_queue.pop();
			HttpCallBackMap::iterator itr = m_HttpCallBack.find(item->path);
			if (itr != m_HttpCallBack.end())
			{
				itr->second(item->path, item->params);
			}
			else
			{
				::printf_s("Unknow http req path:%s\n", item->path.c_str());
			}
			SAFE_DELETE(item);
		}
	}

	void HttpGame::OnRegisterServer()
	{
		RegisterDefaultServer([this](const string &p, HttpParamMap &params, HttpResponse &res){this->PostHttpRequest(p, params, res); });

		HTTP_GAME_REGISTER("/kick", OnKickRequest);
	}

	void HttpGame::OnInit()
	{
	}

	void HttpGame::OnRelease()
	{
	}

	void HttpGame::RegisterServer(string path, HttpCallBack call)
	{
		m_HttpCallBack[path] = call;
	}

	void HttpGame::PostHttpRequest(const string &path, HttpParamMap &params, HttpResponse &res)
	{
		HttpRequestInfo *info = new HttpRequestInfo();
		info->path = path;
		info->params = params;
		m_RequestQueue.Push(info);
		res.AddText("OK_%I64d", TimeUtil::GetCurrentSecond());
	}

	void HttpGame::OnKickRequest(const string &path, HttpParamMap &params)
	{
		HttpParamMap::iterator itr = params.find("user");
		if (itr != params.end())
		{
			::printf_s("OnKickRequest user:%s\n", itr->second.c_str());
			return;
		}

		itr = params.find("roleid");
		if (itr != params.end())
		{
			Int64 roleid = ::_atoi64(itr->second.c_str());
			::printf_s("OnKickRequest roleid:%I64d\n", roleid);
			return;
		}
		::printf_s("OnKickRequest not found user or roleid.\n");
	}
}