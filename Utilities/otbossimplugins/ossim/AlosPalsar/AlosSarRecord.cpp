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

#include <AlosSarRecord.h>
#include <memory.h>

namespace ossimplugins
{

AlosSarRecord::AlosSarRecord(std::string mnemonic):
  _mnemonic(mnemonic)
{
}

AlosSarRecord::~AlosSarRecord()
{
}

AlosSarRecord::AlosSarRecord(const AlosSarRecord& rhs):
  _mnemonic(rhs._mnemonic)
{
}

}
