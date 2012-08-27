#include "GUID.hpp"

uint64 Generate64BitsGUID()
{
    QueryResult result = sDatabase.PQuery("SELECT UUID_SHORT();");
    result->next();

    return result->getUInt64(1);
}
