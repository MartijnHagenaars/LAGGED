#include "Precomp.h"
#include "UniqueID.h"

#include "Utility/Logger.h"

namespace LAG::Utility
{
    static unsigned int counterID = 1;
    unsigned int GetID()
    {
        return counterID++;
    }
}