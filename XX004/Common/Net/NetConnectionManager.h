#ifndef __NetConnectionManager_h__
#define __NetConnectionManager_h__

#include <WinSock2.h>
#include <vector>

namespace XX004
{
	namespace Net
	{
		//�������ӹ���
		class NetServer;
		class NetConnection;
		class NetConnectionThread;
		class NetConnectionManager
		{
			typedef std::vector<NetConnectionThread*> ConnectionVector;
		public:
			//������������
			NetConnectionManager();
			virtual ~NetConnectionManager();

			//����
			void Start();
			
			//ֹͣ
			void Stop();

			//�ȴ���ȫ����
			void Join();

			//���÷����
			inline void SetServer(NetServer *p) { m_pServer = p; }

			//�������
			NetConnection* AddConnection(SOCKET s);

		private:
			//��������
			NetServer *m_pServer;

			//�����߳�
			ConnectionVector m_ConnectionThreads;
		};
	}
}

#endif	//__NetConnectionManager_h__