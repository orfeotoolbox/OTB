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

#include <AlosPalsar/AlosSarLeaderFactory.h>

#include <AlosPalsar/AlosSarDataSetSummary.h>
#include <AlosPalsar/AlosSarFileDescriptor.h>
#include <AlosPalsar/AlosSarPlatformPositionData.h>
#include <AlosPalsar/AlosSarFacilityData.h>

namespace ossimplugins
{

AlosSarLeaderFactory::AlosSarLeaderFactory()
{
  RegisterRecord(17, new AlosSarFacilityData());
  RegisterRecord(3, new AlosSarPlatformPositionData());
//  RegisterRecord(3, new AlosSarMapProjectionData());//
  RegisterRecord(2, new AlosSarDataSetSummary());
  RegisterRecord(1, new AlosSarFileDescriptor());
}

AlosSarLeaderFactory::~AlosSarLeaderFactory()
{

}

}
