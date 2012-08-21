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
#include "Database.hpp"
#include <fstream>
#include <cstdarg>
#include <sstream>
#include <algorithm>
#include <cctype>

Database sDatabase;

Database::Database()
{
}

Database::~Database()
{
    delete Statement;
    delete Connection;
}

void Database::Connect()
{
    std::ifstream CfgFile("WorldServerConfig.conf");

    if(!CfgFile)
        throw std::runtime_error("Cannot open WorldServerConfig.conf");

    std::string Data[4], Buffer;
    int i = 0;

    while(std::getline(CfgFile, Buffer))
    {
        Buffer.erase(Buffer.begin(), find_if(Buffer.begin(), Buffer.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));

        if(Buffer.empty())
            continue;

        if(!std::isalnum(Buffer[0]))
            continue;

        std::stringstream(Buffer) >> Data[i];
        ++i;
    }

    Driver = get_driver_instance();
    Connection = Driver->connect(Data[0], Data[1], Data[2]);
    Connection->setSchema(Data[3]);
    Statement = Connection->createStatement();
}

#ifndef HAVE_VARIADIC_TEMPLATES
    #define MAX_QUERY_LEN 3*1024 // this is so wrong in so many ways

    void Database::PExecute(const char* sql, ...)
    {
        va_list ArgList;
        char Query[MAX_QUERY_LEN];

        va_start(ArgList, sql);
        secure_vsnprintf(Query, MAX_QUERY_LEN, sql, ArgList);
        va_end(ArgList);

        Execute(Query);
    }
#endif

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
#ifndef HAVE_VARIADIC_TEMPLATES
    QueryResult Database::PQuery(const char* sql, ...)
    {
        va_list ArgList;
        char Query[MAX_QUERY_LEN];

        va_start(ArgList, sql);
        secure_vsnprintf(Query, MAX_QUERY_LEN, sql, ArgList);
        va_end(ArgList);

        return this->Query(Query);
    }
#endif
