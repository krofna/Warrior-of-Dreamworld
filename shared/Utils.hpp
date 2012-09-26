/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012 Ryan Lahfa

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
#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <string>
#include <sstream>
#include <boost/format.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/lexical_cast.hpp>

extern boost::random::mt19937 generator;

template<class T>
inline std::string ToString(T What)
{
    return boost::lexical_cast<std::string>(What);
}

inline int urand(int MIN, int MAX)
{
    boost::random::uniform_int_distribution<> dist(MIN, MAX);
    return dist(generator);
}

inline std::string Format(boost::format formater)
{
    return formater.str();
}

inline std::string Format(std::string const& toFormat)
{
    return toFormat;
}

#ifdef HAVE_VARIADIC_TEMPLATES

template<typename T, typename... Values>
inline std::string Format(boost::format formater, T const& val, Values... values)
{
    return Format(formater % val, std::forward<Values>(values)...);
}

template<typename T, typename... Values>
inline std::string Format(std::string const& toFormat, T const& val, Values... values)
{
    return Format(boost::format(toFormat) % val, std::forward<Values>(values)...);
}

#endif

#endif // UTILS_HPP_INCLUDED
