#include "Precomp.h"
#include "UniqueID.h"

namespace LAG::Utility
{
    static unsigned int counterID = 1;
    unsigned int GetID()
    {
        return counterID++;
    }
}