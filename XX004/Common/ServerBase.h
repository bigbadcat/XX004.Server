/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ServerBase.h
* Summary: 服务模块的基类。
*
* Author: XuXiang
* Date: 2018-10-25 19:24
*******************************************************/

#ifndef __ServerBase_h__
#define __ServerBase_h__

namespace XX004
{
	class NetManager;

	//服务模块的基类
	class ServerBase
	{
	public:
		ServerBase();
		virtual ~ServerBase();

		//注册网络消息
		virtual void RegisterNetMessage(NetManager *pMgr) = 0;

		//启动服务
		//step:启动分步计数
		//r:启动进度0-1
		//ret:是否启动完成
		virtual bool OnStart(int step, float &r) = 0;

		//帧更新
		virtual void OnUpdate() = 0;
		
		//秒更新
		virtual void OnUpdatePerSecond() = 0;

		//停止服务
		//step:停止分步计数
		//r:停止进度0-1
		//ret:是否停止完成
		virtual bool OnStop(int step, float &r) = 0;

	protected:


	private:

	};
}

#endif	//__ServerBase_h__
