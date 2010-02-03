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

#include <AlosPalsar/AlosPalsarRecordFactory.h>
#include <ossim/base/ossimTrace.h>

// Static trace for debugging
static ossimTrace traceDebug("AlosPalsarRecordFactory:debug");

namespace ossimplugins
{

AlosPalsarRecordFactory::AlosPalsarRecordFactory()
{
}

AlosPalsarRecordFactory::~AlosPalsarRecordFactory()
{
}

AlosPalsarRecord* AlosPalsarRecordFactory::Instanciate(int id)
{
  if (traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG) << "Intanciate AlosPalsar record:" << id << "\n";
  }
  AlosPalsarRecord* record = _availableRecords[id];
  if (record == NULL)
  {
    return NULL;
  }
  else
  {
    return record->Instanciate();
  }
}

void AlosPalsarRecordFactory::RegisterRecord(int id, AlosPalsarRecord * record)
{
  _availableRecords[id] = record;
}

}
