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


#include "otbSIXSTraits.h"
#include <fstream>
#include <cstdlib>
#include "otbObjectList.h"
#include "itksys/SystemTools.hxx"
#include "otbSpectralResponse.h"

int otbFilterFunctionValuesSpectralResponseTest(int argc, char * argv[])
{

  if ( argc!=4)
   {
     std::cout << argv[0] << std::endl << "\t" << "<Spectrum_filename>"  << "\t" << "<Output_filename>"<<  "\t" << "<Lambda>"<<std::endl;
     return EXIT_FAILURE;
   }

  const std::string inFile(argv[1]);
  char *       outname   = argv[2];
  double step = atof(argv[3]);

  typedef otb::SpectralResponse< double, double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;
  typedef otb::FilterFunctionValues FilterFunctionValuesType;
   //Instantiation
   ResponsePointerType  myResponse=ResponseType::New();
   //Load file into vector
   myResponse->Load(inFile, 100.0);
   //itk::Indent ind;

  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  // Instantiating object
  FilterFunctionValuesType::Pointer          filterFunctionValues=myResponse->GetFilterFunctionValues(step);
  FilterFunctionValuesType::ValuesVectorType object= filterFunctionValues->GetFilterFunctionValues();
  // Writing output file
  std::ofstream file;
  file.open(outname);

  file << "Output vector :" << std::endl;
  for (unsigned int i = 0; i < object.size(); ++i)
    {
    file << object[i] << std::endl;
    }
  file << std::endl;
  file << "L_min :" << filterFunctionValues->GetMinSpectralValue() << std::endl;
  file << "L_max :" << filterFunctionValues->GetMaxSpectralValue() << std::endl;

  file.close();

  return EXIT_SUCCESS;
}



int otbFilterFunctionValuesTest(int argc, char * argv[])
{
  char *       outname   = argv[1];
  const double SIXSStepOfWavelengthSpectralBandValues(0.0025);

  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  // Instantiating object
  FilterFunctionValuesType::Pointer          object = FilterFunctionValuesType::New();
  FilterFunctionValuesType::ValuesVectorType vect;

  for (int i = 5; i < argc; ++i)
    {
    vect.push_back(atof(argv[i]));
    }

  object->SetMinSpectralValue(atof(argv[2]));
  object->SetMaxSpectralValue(atof(argv[3]));
  object->SetUserStep(atof(argv[4]));
  object->SetFilterFunctionValues(vect);

  // Call interpolate
  otb::SIXSTraits::ComputeWavelengthSpectralBandValuesFor6S(
      SIXSStepOfWavelengthSpectralBandValues,
      object);

  // Writing output file
  std::ofstream file;
  file.open(outname);

  file << "Input Vector :" << std::endl;
  for (unsigned int i = 0; i < vect.size(); ++i)
    {
    file << vect[i] << std::endl;
    }
  file << std::endl;
  file << "Output vector :" << std::endl;
  for (unsigned int i = 0; i < object->GetFilterFunctionValues6S().size(); ++i)
    {
    file << object->GetFilterFunctionValues6S()[i] << std::endl;
    }
  file << std::endl;
  file << "L_min :" << object->GetMinSpectralValue() << std::endl;
  file << "L_max :" << object->GetMaxSpectralValue() << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
