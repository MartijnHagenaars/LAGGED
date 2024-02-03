#include "UniqueID.h"

namespace LAG::UniqueID
{
    static unsigned int counterID = 1;
    unsigned int GetID()
    {
        return counterID++;
    }
}