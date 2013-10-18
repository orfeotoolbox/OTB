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

#include <RadarSat/RadarSatRecordFactory.h>

namespace ossimplugins
{
RadarSatRecordFactory::RadarSatRecordFactory()
{
}

RadarSatRecordFactory::~RadarSatRecordFactory()
{
}

RadarSatRecord* RadarSatRecordFactory::Instanciate(int id)
{
	RadarSatRecord* record = _availableRecords[id];
	if(record == NULL)
	{
		return NULL;
	}
	else
	{
		return record->Instanciate();
	}
}

void RadarSatRecordFactory::RegisterRecord(int id, RadarSatRecord * record)
{
	_availableRecords[id] = record;
}
}
