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

#include <RadarSat/RadarSatRecord.h>
#include <memory.h>

namespace ossimplugins
{
RadarSatRecord::RadarSatRecord(std::string mnemonic):
	_mnemonic(mnemonic)
{
}

RadarSatRecord::~RadarSatRecord()
{
}

RadarSatRecord::RadarSatRecord(const RadarSatRecord& rhs):
	_mnemonic(rhs._mnemonic)
{
}
}
