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


#include <fstream>
#include <chrono>
#include <cstdlib>
#include <thread>

#include "otbCoordinateToName.h"
#include "otbCurlHelperStub.h"

int otbCoordinateToNameTest(int argc, char* argv[])
{

  if (argc < 4)
  {
    std::cout << argv[0] << " <lon> <lat> <outputfile>" << std::endl;

    return EXIT_FAILURE;
  }

  const char* outFileName = argv[3];

  otb::CoordinateToName::Pointer conv = otb::CoordinateToName::New();
  conv->SetLon(atof(argv[1]));
  conv->SetLat(atof(argv[2]));

  // only for testing (remove dependency to network conditions)
  otb::CurlHelperStub::Pointer curlStub = otb::CurlHelperStub::New();
  conv->SetCurl(curlStub);

  if ((argc > 4) && atoi(argv[4]) == 1)
  {
    using namespace std::chrono_literals;
    conv->MultithreadOn();
    conv->Evaluate();
    std::this_thread::sleep_for(10s);
  }
  else
  {
    conv->MultithreadOff();
    conv->Evaluate();
  }

  std::string name    = conv->GetPlaceName();
  std::string country = conv->GetCountryName();

  std::cout << "Nearby place: " << name << std::endl;
  std::cout << "Country: " << country << std::endl;

  std::ofstream file;
  file.open(outFileName);
  file << "Nearby place: " << name << std::endl;
  file << "Country: " << country << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
