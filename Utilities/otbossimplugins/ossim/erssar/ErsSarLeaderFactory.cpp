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

#include "erssar/ErsSarLeaderFactory.h"

#include "erssar/ErsSarDataSetSummary.h"
#include "erssar/ErsSarFileDescriptor.h"
#include "erssar/ErsSarMapProjectionData.h"
#include "erssar/ErsSarPlatformPositionData.h"
#include "erssar/ErsSarFacilityData.h"

namespace ossimplugins
{

ErsSarLeaderFactory::ErsSarLeaderFactory()
{
  RegisterRecord(5, new ErsSarFacilityData());
  RegisterRecord(4, new ErsSarPlatformPositionData());
  RegisterRecord(3, new ErsSarMapProjectionData());
  RegisterRecord(2, new ErsSarDataSetSummary());
  RegisterRecord(1, new ErsSarFileDescriptor());
}

ErsSarLeaderFactory::~ErsSarLeaderFactory()
{
  std::map<int, ErsSarRecord*>::iterator i = _availableRecords.begin();

  while (i != _availableRecords.end())
  {
    if ((*i).second)
    {
      delete(*i).second;
      (*i).second = 0;
    }
    ++i;
  }
  _availableRecords.clear();
}
}
