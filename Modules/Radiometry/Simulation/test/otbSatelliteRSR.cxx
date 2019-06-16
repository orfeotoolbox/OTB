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


#include "otbMacro.h"

#include "otbSatelliteRSR.h"

int otbSatelliteRSR(int argc, char * argv[])
{
  typedef otb::SatelliteRSR< double, double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;

  if ( argc!=5 )
  {
    std::cout << argv[0] << std::endl << "\t" << "<RSR_filename>"<< "\t" << "<Nb total band>"<< "\t" << "<lambda>"<< "\t" << "<band>"<< std::endl;
    return EXIT_FAILURE;
  }
  //Instantiation
  ResponsePointerType  myResponse=ResponseType::New();

  myResponse->SetNbBands(atoi(argv[2]));
  //Load file into vector
  const std::string file(argv[1]);
  myResponse->Load(file);
  //itk::Indent ind;
  unsigned int NumBand = atoi(argv[4]);
  double lambda = atof(argv[3]);

  otbLogMacro(Debug, << myResponse);
  otbLogMacro(Info, << "RSR("<<lambda<<","<< NumBand <<")= " << (*myResponse)(lambda, NumBand));

  return EXIT_SUCCESS;
}

int otbSatelliteRSRCheckValue(int argc, char * argv[])
{
  typedef otb::SatelliteRSR< double, double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;

  if ( argc!=6 )
  {
  std::cout << argv[0] << std::endl << "\t" << "<RSR_filename>"<< "\t" << "<Nb total band>"<< "\t"
            << "<lambda>"<< "\t" << "<band>"<< "\t" << "<expected>" << std::endl;
  return EXIT_FAILURE;
  }
  //Instantiation
  ResponsePointerType  myResponse=ResponseType::New();

  myResponse->SetNbBands(atoi(argv[2]));
  //Load file into vector
  const std::string file(argv[1]);
  myResponse->Load(file);
  //itk::Indent ind;
  unsigned int NumBand = atoi(argv[4]);
  double lambda = atof(argv[3]);
  double expected = atof(argv[5]);

  double rsr_value = (*myResponse)(lambda, NumBand);
  
  std::cout << "RSR("<<lambda<<","<< NumBand <<")= " << rsr_value << std::endl;

  if(fabs(rsr_value - expected) > 10e-3) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int otbSatelliteRSRSolarIrradianceCheckValue(int argc, char * argv[])
{
  typedef otb::SatelliteRSR< double, double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;

  if ( argc!=5 )
  {
  std::cout << argv[0] << std::endl << "\t" << "<RSR_filename>"<< "\t" << "<Nb total band>"<< "\t"
            << "<lambda>"<< "\t" << "\t" << "<expected>" << std::endl;
  return EXIT_FAILURE;
  }
  //Instantiation
  ResponsePointerType  myResponse=ResponseType::New();

  myResponse->SetNbBands(atoi(argv[2]));
  //Load file into vector
  const std::string file(argv[1]);
  myResponse->Load(file);
  //itk::Indent ind;
  double lambda = atof(argv[3]);
  double expected = atof(argv[4]);

  double solar_irradiance = (*(myResponse->GetSolarIrradiance()))(lambda);

  std::cout << "SolarIrradiance("<<lambda <<")= " << solar_irradiance << std::endl;
  if(fabs(solar_irradiance - expected) > 10e-3)
    {
    std::cout << "Expected value was = " << expected << std::endl;
    return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}
