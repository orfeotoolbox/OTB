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

#include <AlosPalsar/AlosPalsarLeaderFactory.h>

#include <AlosPalsar/AlosPalsarDataSetSummary.h>
#include <AlosPalsar/AlosPalsarFileDescriptor.h>
#include <AlosPalsar/AlosPalsarPlatformPositionData.h>
#include <AlosPalsar/AlosPalsarRadiometricData.h>
#include <AlosPalsar/AlosPalsarFacilityData.h>

namespace ossimplugins
{

AlosPalsarLeaderFactory::AlosPalsarLeaderFactory()
{
  RegisterRecord(17, new AlosPalsarFacilityData());
  RegisterRecord(5, new AlosPalsarRadiometricData());
  RegisterRecord(3, new AlosPalsarPlatformPositionData());
//  RegisterRecord(3, new AlosPalsarMapProjectionData());//
  RegisterRecord(2, new AlosPalsarDataSetSummary());
  RegisterRecord(1, new AlosPalsarFileDescriptor());
}

AlosPalsarLeaderFactory::~AlosPalsarLeaderFactory()
{

}

}
