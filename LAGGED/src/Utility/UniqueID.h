#pragma once
#include <string>

namespace LAG::Utility
{
    //Returns a number that simply increments.
    //Note: Is NOT thread safe.
    unsigned int GetID();
}
