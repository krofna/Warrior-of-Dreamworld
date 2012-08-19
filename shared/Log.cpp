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
#include "Log.hpp"
#include <iostream>
#include <cstdarg>
#include <cstdlib>

Log sLog;

Log::Log() :
File    ("Log.txt")
{
}

#ifndef HAVE_VARIADIC_TEMPLATES
void Log::Write(const char* format, ...)
{
    va_list ArgList;
    char Query[128];

    va_start(ArgList, format);
    vsnprintf_s(Query, 128, format, ArgList);
    va_end(ArgList);

    boost::mutex::scoped_lock lock(LogMutex);

    std::cout << (char*)Query << "\n";
    File << (char*)Query << "\n";
}
#else
void Log::Write(std::string const& String)
{
    boost::mutex::scoped_lock lock(LogMutex);

    std::cout << String << '\n';
    File << String << '\n';
}
#endif

void Log::Flush()
{
    boost::mutex::scoped_lock lock(LogMutex);

    std::cout << std::flush;
    File << std::flush;
}
