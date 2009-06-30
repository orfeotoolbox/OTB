#include <AlosSarRecordFactory.h>

AlosSarRecordFactory::AlosSarRecordFactory()
{
}

AlosSarRecordFactory::~AlosSarRecordFactory()
{
}

AlosSarRecord* AlosSarRecordFactory::Instanciate(int id)
{
	AlosSarRecord* record = _availableRecords[id];
	if(record == NULL)
	{
		return NULL;
	}
	else
	{
		return record->Instanciate();
	}
}

void AlosSarRecordFactory::RegisterRecord(int id, AlosSarRecord * record)
{
	_availableRecords[id] = record;
}