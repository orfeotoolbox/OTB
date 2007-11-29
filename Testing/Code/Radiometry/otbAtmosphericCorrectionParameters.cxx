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

int otbFilterFunctionValuesTest(int argc, char * argv[])
{
  try
    {   
      char * outname   = argv[1];

      typedef otb::FilterFunctionValues FilterFunctionValuesType;
      // Instantiating object
      FilterFunctionValuesType::Pointer object = FilterFunctionValuesType::New();
      FilterFunctionValuesType::ValuesVectorType vect;

      for(int i=5; i<argc; i++)
	{
	  vect.push_back(atof(argv[i]));
	}
 
//      bool bumbo = object->SetParameters(atof(argv[2]), atof(argv[3]), atof(argv[4]), vect);
        const double SIXSStepOfWavelenghtSpectralBandValues(0.0025);
/*
        otb::SIXSTraits::ComputeWavelenghtSpectralBandValuesFor6S(
        ,
                WavelenghtSpectralType& WavelenghtSpectralBand
        );
*/

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
      for (unsigned int i=0; i<object->GetFilterFunctionValues().size(); i++)
	{
	  file<< object->GetFilterFunctionValues()[i] <<std::endl;
	}
      file<<std::endl;
      file<<"L_min :"<<object->GetMinSpectralValue()<<std::endl;
      file<<"L_max :"<<object->GetMaxSpectralValue()<<std::endl;
      
      file.close();
    }
  
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
