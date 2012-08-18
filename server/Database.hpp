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
#ifndef DATABASE_H
#define DATABASE_H

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <memory>

typedef std::unique_ptr<sql::ResultSet> QueryResult;

class Database
{
public:
    Database();
    ~Database();

    void Connect();
    
    QueryResult Query(const char* sql);
    void Execute(const char* sql);
    
    #ifdef HAVE_VARIADIC_TEMPLATES
    // TODO: Merge Format to make a free function
    std::string Format(std::string const& toFormat);
    template<typename Value, typename... Values>
    std::string Format(std::string const& toFormat, Value const& val, Values... values);
    
    template<typename... Values>
    void PExecute(std::string const& toFormat, Values... values);
    template<typename... Values>
    QueryResult PQuery(std::string const& toFormat, Values... values);
    #else
    
    void PExecute(const char* sql, ...);
    QueryResult PQuery(const char* sql, ...);
    
    #endif
    

private:
    sql::Driver* Driver;
    sql::Connection* Connection;
    sql::Statement* Statement;
};

#ifdef HAVE_VARIADIC_TEMPLATES

std::string Database::Format(std::string const& toFormat)
{
    return toFormat;
}
// TODO: Need more work.
template<typename Value, typename... Values>
std::string Database::Format(std::string const& toFormat, Value const& val, Values... values)
{
    std::ostringstream QueryStr;

    size_t placeholderPos = toFormat.find('%');
    bool useValues = false, writeValues = true;

    if (placeholderPos != std::string::npos)
    {
        if(toFormat[placeholderPos + 1] == '%')
        {
            toFormat.erase(toFormat.begin() + placeholderPos, toFormat.begin() + placeholderPos);
            writeValues = false;
        }
        useValues = true;
    }

    if (useValues)
    {
        if (writeValues)
            QueryStr << toFormat.substr(0, placeholderPos) << val << Format(toFormat.substr(placeholderPos+1), values...);
        else
            QueryStr << toFormat.substr(0, placeholderPos + 1) << Format(toFormat.substr(placeholderPos + 2), val, values...);
    }

    else
        QueryStr << toForma;t

    return QueryStr.str();
}

template<typename... Values>
void Database::PExecute(std::string const& sql, Values... values)
{
    Execute(Format(sql, values...).c_str());
}
template<typename Value, typename... Values>
void Database::PQuery(std::string const& sql, Values... values)
{
    Query(Format(sql, values...).c_str());
}
#endif
extern Database sDatabase;

#endif
