/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetManagerGate.h
* Summary: 网关网络管理。
*
* Author: XuXiang
* Date: 2018-12-03 19:42
*******************************************************/

#ifndef __NetManagerGate_h__
#define __NetManagerGate_h__

#include "NetManagerBase.h"
#include <vector>
using namespace std;

namespace XX004
{
	//网关转发配置
	class GateRouteConfig
	{
	public:
		//构造函数
		GateRouteConfig(int b, int e, RemoteType t);

		//获取起始号
		inline int GetBegin()const { return m_Begin; }

		//获取结束号
		inline int GetEnd()const { return m_End; }

		//获取转发目标
		inline RemoteType GetTarget()const { return m_Target; }

	private:

		//消息起始号
		int m_Begin;

		//消息结束号
		int m_End;

		//转发目标
		RemoteType m_Target;
	};
	typedef vector<GateRouteConfig> GateRouteConfigVector;

	class NetManagerGate : public NetManagerBase
	{
	public:
		NetManagerGate();
		virtual ~NetManagerGate();

	protected:
		//添加接收到的数据
		virtual void OnAddRecvData(NetDataItem *item);

		//创建内部连接信息
		virtual void OnCreateInternalInfo(std::vector<int> &infos);

	private:

		//转发配置列表
		GateRouteConfigVector m_GateRouteConfig;
	};
}

#endif