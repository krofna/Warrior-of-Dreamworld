#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdlib>
#include <sstream>

inline int urand(int MIN, int MAX)
{
    return (rand()%(MAX-MIN+1) + MIN);
}

std::string IntToString(int Integer);
int StringToInt(const std::string& String);

#endif