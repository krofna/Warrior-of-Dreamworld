/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012  Mislav Blazevic

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "Database.h"
#include <fstream>
#include <cstdarg>

Database sDatabase;

Database::Database()
{
}

Database::~Database()
{
    delete Statement;
    delete Connection;
}

// TODO: Allow #comments in cfg file
void Database::Connect()
{
    std::ifstream CfgFile("WorldServerConfig.conf");
    std::string Address, Username, Password, DatabaseName;

    CfgFile >> Address >> Username >> Password >> DatabaseName;
    
    Driver = get_driver_instance();
    Connection = Driver->connect(Address, Username, Password);
    Connection->setSchema(DatabaseName);
    Statement = Connection->createStatement();
}

#define MAX_QUERY_LEN 3*1024 // this is so wrong in so many ways

void Database::PExecute(const char* sql, ...)
{
    va_list ArgList;
    char Query[MAX_QUERY_LEN];

    va_start(ArgList, sql);
    vsnprintf(Query, MAX_QUERY_LEN, sql, ArgList);
    va_end(ArgList);

    Execute(Query);
}

void Database::Execute(const char* sql)
{
    Statement->execute(sql);
}

QueryResult Database::Query(const char* sql)
{
    std::auto_ptr<sql::PreparedStatement> PStatement(Connection->prepareStatement(sql));
    QueryResult Result(PStatement->executeQuery());

    return Result;
}

QueryResult Database::PQuery(const char* sql, ...)
{
    va_list ArgList;
    char Query[MAX_QUERY_LEN];

    va_start(ArgList, sql);
    vsnprintf(Query, MAX_QUERY_LEN, sql, ArgList);
    va_end(ArgList);

    return this->Query(Query);
}