#include <ossim/imaging/EnvisatAsar/EnvisatAsarRecordFactory.h>

EnvisatAsarRecordFactory::EnvisatAsarRecordFactory()
{
}

EnvisatAsarRecordFactory::~EnvisatAsarRecordFactory()
{
}

EnvisatAsarRecord* EnvisatAsarRecordFactory::Instanciate(std::string id)
{
	EnvisatAsarRecord* record = _availableRecords[id];
	if(record == NULL)
	{
		return NULL;
	}
	else
	{
		return record->Instanciate();
	}
}

void EnvisatAsarRecordFactory::RegisterRecord(std::string id, EnvisatAsarRecord * record)
{
	_availableRecords[id] = record;
}