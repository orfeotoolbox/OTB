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

#include <AlosSarLeaderFactory.h>

#include <AlosSarDataSetSummary.h>
#include <AlosSarFileDescriptor.h>
//#include <AlosSarMapProjectionData.h>//
#include <AlosSarPlatformPositionData.h>
#include <AlosSarFacilityData.h>

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
