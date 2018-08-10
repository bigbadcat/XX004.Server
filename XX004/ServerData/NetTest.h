#ifndef __NetTest_h__
#define __NetTest_h__

#include <thread>
#include <WinSock2.h>

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

		static void ListenFun(NetTest *test);
		static void ClientFun(NetTest *test);

		std::thread m_listen_thread;
		std::thread m_client_thread;
		bool m_running;
	};
}

#endif	//__NetTest_h__