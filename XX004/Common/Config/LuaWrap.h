/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: LuaWrap.h
* Summary: Lua配置基类。
*
* Author: XuXiang
* Date: 2019-01-24 20:57
*******************************************************/

#ifndef __LuaWrap_h__
#define __LuaWrap_h__

#include <string>
using namespace std;

struct lua_State;

namespace XX004
{
	class LuaWrap
	{
	public:
		LuaWrap();
		virtual ~LuaWrap();

		//初始化
		//L:Lua状态，必须保证栈顶是配置项table，函数调用后保证Lua堆栈不变
		virtual void Init(lua_State *L) = 0;

		//二次解析
		virtual void OnParse();

		//创建新对象
		template<class T>
		static LuaWrap* OnNew();

		//获取Lua状态
		static lua_State* GetLuaState();

		//释放Luanda
		static void ReleaseLua();

		//执行Lua文件
		//L:Lua状态
		//file:Lua文件
		//resetstack:是否保值Lua堆栈不变
		static bool DoLuaFile(lua_State *L, const char *file, bool resetstack=true);

	protected:

		static bool ReadBool(lua_State *L, char *name);
		static int ReadInt(lua_State *L, char *name);
		static float ReadFloat(lua_State *L, char *name);
		static double ReadDouble(lua_State *L, char *name);
		static string ReadString(lua_State *L, char *name);
		static void ReadString(lua_State *L, char *name, string &str);

	private:
		static lua_State *LuaState;
	};

	template<class T>
	LuaWrap* LuaWrap::OnNew()
	{
		T *t = new T();
		LuaWrap *wrap = dynamic_cast<LuaWrap*>(t);
		return wrap;
	}
}

#endif
