#include "MySQLTest.h"
#include <iostream>
#include <string>

using namespace std;

namespace XX004
{
	MySQLTest::MySQLTest()
	{
	}

	MySQLTest::~MySQLTest()
	{
	}

	void MySQLTest::DoTest()
	{
		cout << "MySQLTest::DoTest()" << endl;

		MYSQL mysql;
		mysql_init(&mysql);
		mysql_real_connect(&mysql, "localhost", "root", "1234", "xx004", 3306, NULL, 0);

		try
		{
			//MySQL命令
			char sql[128];
			sprintf_s(sql, "call sp_select_role(%I64d);", 214748364734);
			if(mysql_query(&mysql, sql) != 0)
			{
				const char *error_str = mysql_error(&mysql);
				if(error_str == NULL)
				{
					throw string("MySQL is error!");
				}
				else
				{
					throw string(error_str);
				}
			}

			MYSQL_RES *result = mysql_store_result(&mysql);
			if(!result)
				throw string("MySQL not result!");

			//获取字段数量
			int num_fields = mysql_num_fields(result);
			if(0 == num_fields)
				throw string("MySQL fields number is 0!");

			//获取字段名
			MYSQL_FIELD *fields = mysql_fetch_fields(result);
			if(!fields)
				throw string("MySQL fields fetch is error!");

			for(int i = 0; i < num_fields; i++)
			{
				//cout << "field " << i << " name is " << fields[i].name << endl;
				cout << fields[i].name << " ";
			}
			cout << endl;

			//逐行读取数据
			while (MYSQL_ROW row = mysql_fetch_row(result)) //获取整条数据内容
			{
				for (int i = 0; i < num_fields; i++)
				{
					if (NULL == row[i])
					{
						cout << "NULL ";
					}
					else
					{
						cout <<row[i] << " ";
					}

					if (i == num_fields - 1)
					{
						//__int64 ct = 0;
						//if (strcmp(row[i], "NULL") != 0)
						//{
						//	ct = ::_atoi64(row[i]);
						//	ct++;
						//}
					}
				}
				cout << endl;
			}
			mysql_free_result(result);
			result = NULL;
			ClearResult(&mysql);

			mysql_query(&mysql, "START TRANSACTION");

			//更新玩家数据
			{
				__int64 id = 214748364734;
				string name = "kkk";
				int level = 10;
				int exp = 300;
				int money = 1000;
				__int64 create_time = 14214748364734;
				sprintf_s(sql, "call sp_insert_update_role(%I64d,\"%s\",%d,%d,%d,%I64d);", id, name.c_str(), level, exp, money, create_time);
				//if (mysql_query(&mysql, sql) != 0) { MySQLTest::ThrowError(&mysql); }
				CHECK_MYSQL_QUERY(&mysql, sql)
			}
			
			//更新技能数据
			{
				__int64 id = 214748364734;
				int skill = 2001;
				int level = 15;
				__int64 create_time = 14214748364734;
				sprintf_s(sql, "call sp_insert_update_skill(%I64d,%d,%d);", id, skill, level);
				if (mysql_query(&mysql, sql) != 0)
				{
					const char *error_str = mysql_error(&mysql);
					if (error_str == NULL)
					{
						throw string("MySQL is error!");
					}
					else
					{
						throw string(error_str);
					}
				}
			}
			
			mysql_query(&mysql, "COMMIT");

			cout << "MySQL is OK." << endl;
		}
		catch(string &error_msg)
		{
			cout << error_msg << endl;
			mysql_query(&mysql, "ROLLBACK");
		}
		catch(...)
		{
			cout << "MySQL operation is error!" << endl;
			mysql_query(&mysql, "ROLLBACK");
		}

		mysql_close(&mysql);
	}

	void MySQLTest::ClearResult(MYSQL *mysql)
	{
		while (!mysql_next_result(mysql))
		{
			MYSQL_RES *result = mysql_store_result(mysql);
			mysql_free_result(result);
		}
	}

	void MySQLTest::ThrowError(MYSQL *mysql)
	{
		const char *error_str = mysql_error(mysql);
		if (error_str == NULL)
		{
			throw string("MySQL is error!");
		}
		else
		{
			throw string(error_str);
		}
	}
}