#ifndef __MySQLTest_h__
#define __MySQLTest_h__

#include <WinSock2.h>
#include <mysql.h>

namespace XX004
{
#define CHECK_MYSQL_QUERY(mysql, sql) if (mysql_query(mysql, sql) != 0) { MySQLTest::ThrowError(mysql); }

	class MySQLTest
	{
	public:
		MySQLTest();
		~MySQLTest();

		void DoTest();

		//������
		static void ClearResult(MYSQL *mysql);

		//�׳��쳣����
		static void ThrowError(MYSQL *mysql);
	private:

	};
}

#endif	//__MySQLTest_h__
