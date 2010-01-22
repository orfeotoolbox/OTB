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

#include <AlosPalsar/AlosSarRecordFactory.h>
#include <ossim/base/ossimTrace.h>

// Static trace for debugging
static ossimTrace traceDebug("AlosSarRecordFactory:debug");

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
  if (traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)<< "Intanciate AlosPalsar record:" << id << "\n";
  }
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
