#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <string>
#include <sstream>

inline std::string Format(std::string const& toFormat)
{
    return toFormat;
}
// TODO: Need more work.
template<typename Value, typename... Values>
inline std::string Format(std::string toFormat, Value const& val, Values... values)
{
    std::ostringstream QueryStr;

    size_t placeholderPos = toFormat.find('%');
    bool useValues = false, writeValues = true, itsEnd = false;

    if (placeholderPos != std::string::npos)
    {
        if(toFormat[placeholderPos + 1] == '%')
        {
            toFormat.erase(toFormat.begin() + placeholderPos, toFormat.begin() + placeholderPos);
            writeValues = false;
        }
        else
        {
            toFormat.erase(toFormat.begin() + placeholderPos, toFormat.begin() + placeholderPos + 1);
        }

        if ((placeholderPos + 1) >= toFormat.size())
            itsEnd = true;
        useValues = true;
    }

    if (useValues)
    {
        if (writeValues)
        {
            if (itsEnd)
                QueryStr << toFormat.substr(0, placeholderPos) << val;
            else
                QueryStr << toFormat.substr(0, placeholderPos) << val << Format(toFormat.substr(placeholderPos+1), values...);
        }
        else
            QueryStr << toFormat.substr(0, placeholderPos + 1) << Format(toFormat.substr(placeholderPos + 2), val, values...);
    }

    else
        QueryStr << toFormat;

    return QueryStr.str();
}

#endif // UTILS_HPP_INCLUDED
