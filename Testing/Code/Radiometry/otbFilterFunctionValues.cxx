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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbSIXSTraits.h"
#include <fstream>
#include <cstdlib>

int otbFilterFunctionValuesTest(int argc, char * argv[])
{
  char * outname   = argv[1];
  const double SIXSStepOfWavelenghtSpectralBandValues(0.0025);

  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  // Instantiating object
  FilterFunctionValuesType::Pointer object = FilterFunctionValuesType::New();
  FilterFunctionValuesType::ValuesVectorType vect;

  for(int i=5; i<argc; i++)
    {
      vect.push_back(atof(argv[i]));
    }

  object->SetMinSpectralValue(atof(argv[2]));
  object->SetMaxSpectralValue(atof(argv[3]));
  object->SetUserStep(atof(argv[4]));
  object->SetFilterFunctionValues(vect);

  // Call interpolate
  otb::SIXSTraits::ComputeWavelenghtSpectralBandValuesFor6S(
							    SIXSStepOfWavelenghtSpectralBandValues,
							    object);


  // Writing output file
  std::ofstream file;
  file.open(outname);

  file <<"Input Vector :"<<std::endl;
  for (unsigned int i=0; i<vect.size(); i++)
    {
      file<< vect[i] <<std::endl;
    }
  file<<std::endl;
  file<<"Output vector :"<<std::endl;
  for (unsigned int i=0; i<object->GetFilterFunctionValues6S().size(); i++)
    {
      file<< object->GetFilterFunctionValues6S()[i] <<std::endl;
    }
  file<<std::endl;
  file<<"L_min :"<<object->GetMinSpectralValue()<<std::endl;
  file<<"L_max :"<<object->GetMaxSpectralValue()<<std::endl;

  file.close();


  return EXIT_SUCCESS;
}
