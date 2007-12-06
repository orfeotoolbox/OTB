/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include "otbAtmosphericCorrectionParameters.h"

int otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms(int argc, char * argv[])
{
  typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms  AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsType;
  typedef otb::AtmosphericCorrectionParameters AtmosphericCorrectionParametersType;
  typedef AtmosphericCorrectionParametersType::AerosolModelType AerosolModelType;
  /*
  // Instantiating object
  AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsType::Pointer object = AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsType::New();
  AtmosphericCorrectionParametersType param; // =  otbAtmosphericCorrectionParameters::New();
  AerosolModelType aerosolModel;
  
  
  
  AerosolModelType AerosolModel;
  
  param->SetSolarZenithalAngle(static_cast<double>(::atof(argv[])));
  param->SetSolarAzimutalAngle(static_cast<double>(::atof(argv[])));
  param->SetViewingZenithalAngle(static_cast<double>(::atof(argv[])));
  param->SetViewingAzimutalAngle(static_cast<double>(::atof(argv[])));
  param->SetMonth(::atoi(argv[]));
  param->SetDay(::atoi(argv[]));
  param->SetAtmosphericPressure(static_cast<double>(::atof(argv[]))); 
  param->SetWaterVaporAmount(static_cast<double>(::atof(argv[])));
  param->SetOzoneAmount(static_cast<double>(::atof(argv[])));
  aerosolModel = static_cast<AerosolModelType>(::atoi(argv[]));
  param->SetAerosolModel(aerosolModel);
  param->SetAerosolOptical(static_cast<double>(::atof(argv[])));
  */
  return EXIT_SUCCESS;
}
