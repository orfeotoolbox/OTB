#include <ossim/imaging/RadarSat/RadarSatRecordFactory.h>

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