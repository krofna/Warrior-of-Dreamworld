#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <string>
#include <sstream>
#include <boost/format.hpp>

inline std::string Format(boost::format formater)
{
    return formater.str();
}
inline std::string Format(std::string const& toFormat)
{
    return toFormat;
}

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

#endif // UTILS_HPP_INCLUDED
