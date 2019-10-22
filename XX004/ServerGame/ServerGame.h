/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerGame.h
* Summary: 游戏服务模块。
*
* Author: XuXiang
* Date: 2019-10-02 11:05
*******************************************************/

#ifndef __ServerGame_h__
#define __ServerGame_h__

#include <vector>
#include <map>
#include <Frame/ServerBase.h>
#include <Frame/ModuleBase.h>
using namespace std;

namespace XX004
{
	typedef vector<ModuleBase*> ModuleVector;
	class LoginModule;
	class HttpGame;

	class ServerGame : public ServerBase
	{
	public:
		ServerGame();
		virtual ~ServerGame();

		//获取模块 若需要频繁使用，建议缓存结果，在ServerGame对象销毁后返回值无效
		template<class T>
		T* GetModule();

	protected:
		//注册网络消息
		virtual void RegisterNetMessage(NetManagerBase *pMgr);

		//注册存储消息
		virtual void RegisterStorageMessage(StorageManager *pMgr);

		//初始化配置列表
		virtual void OnAddConfig(vector<ModuleConfig*> &cfgs);

		//启动服务
		virtual bool OnInitStep(int step);

		//停止服务
		virtual bool OnReleaseStep(int step);

		//帧更新
		virtual void OnUpdate();

		//秒更新
		virtual void OnUpdatePerSecond();

		//收到命令
		virtual void OnCommand(const std::string& cmd, const std::vector<std::string> &param);

	private:
		void InitModules();

		void OnConnect(NetDataItem *item);
		void OnDisconnect(NetDataItem *item);

		//模块列表
		ModuleVector m_Modules;

		//登陆模块
		LoginModule *m_LoginModule;

		//HTTP服务
		HttpGame *m_HttpGame;
	};

	template<class T>
	T* ServerGame::GetModule()
	{
		for (ModuleVector::iterator itr = m_Modules.begin(); itr != m_Modules.end(); ++itr)
		{
			ModuleBase *m = (*itr);
			T *t = dynamic_cast<T*>(m);
			if (t != NULL)
			{
				return t;
			}
		}
		return NULL;
	}
}

#endif