/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbProspectModel.h"

#include <iostream>
#include <fstream>

int otbProspectTransTest(int argc, char* argv[])
{

  if (argc != 8)
  {
    std::cout << "Wrong number of arguments !" << std::endl;
    return EXIT_FAILURE;
  }

  double Cab        = static_cast<double>(atof(argv[1]));
  double Car        = static_cast<double>(atof(argv[2]));
  double CBrown     = static_cast<double>(atof(argv[3]));
  double Cw         = static_cast<double>(atof(argv[4]));
  double Cm         = static_cast<double>(atof(argv[5]));
  double N          = static_cast<double>(atof(argv[6]));
  char*  OutputName = argv[7];

  typedef otb::ProspectModel  PropectType;
  typedef otb::LeafParameters LeafParametersType;

  LeafParametersType::Pointer leafParams = LeafParametersType::New();
  PropectType::Pointer        prospect   = PropectType::New();

  leafParams->SetCab(Cab);
  leafParams->SetCar(Car);
  leafParams->SetCBrown(CBrown);
  leafParams->SetCw(Cw);
  leafParams->SetCm(Cm);
  leafParams->SetN(N);

  prospect->SetInput(leafParams);
  prospect->Update();

  std::ofstream outputFile(OutputName, std::ios::out);
  for (unsigned int i = 0; i < prospect->GetTransmittance()->Size(); ++i)
  {
    outputFile << prospect->GetTransmittance()->GetResponse()[i].second << std::endl;
  }


  return EXIT_SUCCESS;
}
