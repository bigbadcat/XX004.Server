/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 中控程序入口。
*
* Author: XuXiang
* Date: 2019-09-25 16:02
*******************************************************/

#include <vld.h>
#include <iostream>
#include <WinSock2.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>
#include <thread>
#include <string>
#include <map>
#include <Util/StringUtil.h>
#include <MySQL/MySQLWrap.h>
#include <Macro.h>
using namespace std;
using namespace XX004;

struct event_base* base = NULL;
struct evhttp *http_server = NULL;
thread t_http_server;
bool b_run;

typedef map<string, string> HttpParamMap;

std::string http_get_params(const std::string &uri, HttpParamMap &params)
{
	size_t pos = uri.find_first_of('?');
	string path = uri.substr(0, pos);
	string p_str = uri.substr(pos + 1);

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

void http_request_login(struct evhttp_request *req, void *arg)
{
	string uri(evhttp_request_get_uri(req));
	HttpParamMap params_set;
	string path = http_get_params(uri, params_set);
	string user = params_set["user"];
	string pwd = params_set["pwd"];
	cout << "http_request cmd:" << path << " user:" << user << " pwd:" << pwd << endl;

	//回应
	struct evbuffer *retbuff = evbuffer_new();
	evbuffer_add_printf(retbuff, "Hello %s!", user.c_str());
	evhttp_send_reply(req, HTTP_OK, "Client", retbuff);
	evbuffer_free(retbuff);
	retbuff = NULL;
}

void http_server_process()
{
	base = event_base_new();
	printf("Using Libevent with backend method <%s>.\n", event_base_get_method(base));	//获取当前使用了哪种 I/O 模型，Windows 下仅显示 win32

	short http_port = 8081;
	char *http_addr = "0.0.0.0";
	http_server = evhttp_new(base);
	evhttp_bind_socket(http_server, http_addr, http_port);

	evhttp_set_cb(http_server, "/login", http_request_login, NULL);

	while (b_run)
	{
		int r = event_base_loop(base, EVLOOP_NONBLOCK);
		chrono::milliseconds dura(100);
		this_thread::sleep_for(dura);
	}

	evhttp_free(http_server);
	http_server = NULL;

	event_base_free(base);
	base = NULL;
}

void start_http_server()
{
	b_run = true;
	t_http_server = thread(http_server_process);
}

void stop_http_server()
{
	b_run = false;
	if (t_http_server.joinable())
	{
		t_http_server.join();
	}
}

void wait_quit()
{
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
		if (cmd.compare("/q") == 0)
		{
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	WSADATA wsa_data;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);

	////http server
	//start_http_server();
	//wait_quit();
	//stop_http_server();

	{
		//MySQL "localhost", "root", "1234", "xx004", 3306
		MySQLWrap mysql;
		mysql.Init("localhost", "root", "1234", "xx004", 3306);

		char sql[64];
		Int64 roleid = 1000100000004;
		sprintf_s(sql, "call sp_select_role(%I64d);", roleid);

		auto ret = mysql.Query(sql);
		if (ret->GetRecord())
		{
			string name(ret->GetString("name"));
			int prof = ret->GetInt("prof");
			int level = ret->GetInt("level");
			printf_s("name:%s prof:%d level:%d\n", name.c_str(), prof, level);
		}
	}

	//release
	WSACleanup();
	
	system("pause");
	return 0;
}