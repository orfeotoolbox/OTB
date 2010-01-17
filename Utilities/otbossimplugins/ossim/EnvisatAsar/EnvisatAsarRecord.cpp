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

#include <EnvisatAsar/EnvisatAsarRecord.h>
#include <memory.h>

namespace ossimplugins
{
EnvisatAsarRecord::EnvisatAsarRecord(std::string mnemonic):
	_mnemonic(mnemonic)
{
}

EnvisatAsarRecord::~EnvisatAsarRecord()
{
}

EnvisatAsarRecord::EnvisatAsarRecord(const EnvisatAsarRecord& rhs):
	_mnemonic(rhs._mnemonic)
{
}
}
