#include "SQLConnector.h"
#include <iostream>
#include <string>
#include <mysql.h>

#pragma comment(lib, "libmySQL.lib")

using namespace std;


void SQLConnector::CheckConnect()
{
	MYSQL MySQL; 
	mysql_init(&MySQL);

	if (!mysql_real_connect(&MySQL, "localhost", "root", "qwe123", NULL, 3306, NULL, 0))
	{
		cout << "error\n";
	}
	else
	{
		cout << "success\n";
	}

	mysql_close(&MySQL);
}
