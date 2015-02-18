//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "erssar/ErsSarRecord.h"
#include <memory.h>

namespace ossimplugins
{

ErsSarRecord::ErsSarRecord(std::string mnemonic):
    _mnemonic(mnemonic)
{
}

ErsSarRecord::~ErsSarRecord()
{
}

ErsSarRecord::ErsSarRecord(const ErsSarRecord& rhs):
    _mnemonic(rhs._mnemonic)
{
}
}
