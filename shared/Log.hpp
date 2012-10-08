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
#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include "Config.hpp"

// Thread safe output
class Log
{
public:
    Log();
    #ifndef HAVE_VARIADIC_TEMPLATES
    void Write(const char* format, ...);
    #else
    void Write(std::string const& String);
    template<typename... Values>
    void Write(std::string const& String, Values... values);
    #endif

    template<typename Value>
    Log& operator <<(Value const& val);
    void Flush();

private:
    std::ofstream File;
    boost::mutex LogMutex;
};

#ifdef HAVE_VARIADIC_TEMPLATES
template<typename... Values>
void Log::Write(std::string const& String, Values... values)
{
    std::string Formated = Format(String, values...);
    boost::mutex::scoped_lock lock(LogMutex);

    std::cout << Formated << '\n';
    File << Formated << '\n';
}
#endif

template<typename Value>
Log& Log::operator <<(Value const& val)
{
    boost::mutex::scoped_lock lock(LogMutex);

    std::cout << val;
    File << val;
    return *this;
}

extern Log sLog;

#endif
