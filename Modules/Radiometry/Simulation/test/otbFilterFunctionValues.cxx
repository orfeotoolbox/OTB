/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



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
