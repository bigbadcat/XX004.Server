/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: main.cpp
* Summary: 网关程序入口。
*
* Author: XuXiang
* Date: 2018-08-10 22:27
*******************************************************/

#include "MainGate.h"
#include <iostream>
#include <vld.h>
//#include "Util/StringUtil.h"
using namespace std;


int main(int argc, char *argv[])
{
	int ret = 0;
	{
		XX004::MainGate server;
		ret = server.Run();
	}

	//cout << XX004::StringUtil::Trim(string("")) << "-" << endl;
	//cout << XX004::StringUtil::Trim(string("abc")) << "-" << endl;
	//cout << XX004::StringUtil::Trim(string(" def")) << "-" << endl;
	//cout << XX004::StringUtil::Trim(string("ghi ")) << "-" << endl;
	//cout << XX004::StringUtil::Trim(string(" jkl ")) << "-" << endl;
	//cout << XX004::StringUtil::Trim(string(" mn  o ")) << "-" << endl;

	//string s = " 554 852 5861 nb 654 631 58 ";
	//vector<string> vs = XX004::StringUtil::Split(s, string(" "));
	//vector<int> vi = XX004::StringUtil::SplitToInt(s, string(" "));

 	system("pause");
	return ret;
}