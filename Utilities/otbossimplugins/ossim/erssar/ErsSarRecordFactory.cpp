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

#include "erssar/ErsSarRecordFactory.h"

namespace ossimplugins
{

ErsSarRecordFactory::ErsSarRecordFactory()
{
}

ErsSarRecordFactory::~ErsSarRecordFactory()
{
}

ErsSarRecord* ErsSarRecordFactory::Instanciate(int id)
{
  ErsSarRecord* record = _availableRecords[id];
  if (record == NULL)
  {
    return NULL;
  }
  else
  {
    return record->Instanciate();
  }
}

void ErsSarRecordFactory::RegisterRecord(int id, ErsSarRecord * record)
{
  _availableRecords[id] = record;
}
}
