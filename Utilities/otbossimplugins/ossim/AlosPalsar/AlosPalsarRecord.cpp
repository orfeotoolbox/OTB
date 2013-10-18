//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <AlosPalsar/AlosPalsarRecord.h>
#include <memory.h>

namespace ossimplugins
{

AlosPalsarRecord::AlosPalsarRecord(std::string mnemonic):
    _mnemonic(mnemonic)
{
}

AlosPalsarRecord::~AlosPalsarRecord()
{
}

AlosPalsarRecord::AlosPalsarRecord(const AlosPalsarRecord& rhs):
    _mnemonic(rhs._mnemonic)
{
}

}
