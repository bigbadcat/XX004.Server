#ifndef __NetTest_h__
#define __NetTest_h__

#include <thread>
#include <vector>
#include <WinSock2.h>
#include "Net/NetServer.h"

namespace XX004
{
	class NetTest
	{
	public:
		NetTest();
		~NetTest();
		void Run();
		void Stop();

	private:

		//static void ListenFun(NetTest *test);
		static void ClientFun(NetTest *test);

		//Net::NetListener m_listerner;
		Net::NetServer m_Server;
		
		//std::thread m_listen_thread;
		std::vector<std::thread> m_client_thread;
		bool m_running;

		//此数据被多个线程访问 要加锁
		SOCKET m_client_socket;
	};
}

#endif	//__NetTest_h__