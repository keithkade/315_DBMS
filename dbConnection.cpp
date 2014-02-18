#pragma once

#include "rdbms.h"
#include "parser.h"
#include "dbConnection.h"

using namespace std;

DBConnection::DBConnection()
{
	par.setDatabasePtr(&db);
}