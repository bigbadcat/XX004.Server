#ifndef __NetServer_h__
#define __NetServer_h__

#include <iostream>
#include <string>

using namespace std;

namespace XX004
{
	namespace Net
	{
		//���紦��ӿ�
		class INetProcesser
		{
		public:

		};

		//������������
		class NetServer
		{
		public:
			//��������ģ�顣 ipaddress:IP��ַ�� port:�˿ںš�
			void Start(string ipaddress, int port);
		};
	}
}

#endif	//__NetServer_h__