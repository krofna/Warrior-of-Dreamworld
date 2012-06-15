#include "Utilities.h"
#include <iostream>

std::string IntToString(int Integer)
{
    std::stringstream stream;
    stream << Integer;
    return stream.str();
}

int StringToInt(const std::string& String)
{
    std::stringstream buffer(String);
    int a;
    buffer >> a;
    return a;
}