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

#include <AlosSarRecordFactory.h>

namespace ossimplugins
{

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

}
