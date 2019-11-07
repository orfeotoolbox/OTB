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


// This example will show how to retrieve the longitude and latitude from
// a place using the name of the city or the address. For that, we will
// use the \doxygen{otb}{PlaceNameToLonLat} class.

#include "otbPlaceNameToLonLat.h"

int main(int argc, char* argv[])
{

  if (argc != 2)
  {
    std::cout << argv[0] << " <place name> " << std::endl;

    return EXIT_FAILURE;
  }

  // You instantiate the class and pass the name you want to look for as a
  // std::string to the SetPlaceName method.
  //
  // The call to evaluate will trigger the retrival process.

  otb::PlaceNameToLonLat::Pointer pn2LL = otb::PlaceNameToLonLat::New();
  pn2LL->SetPlaceName(std::string(argv[1]));
  pn2LL->Evaluate();

  // To get the data, you can simply call the GetLon and GetLat methods.

  double lon = pn2LL->GetLon();
  double lat = pn2LL->GetLat();

  std::cout << "Latitude: " << lat << std::endl;
  std::cout << "Longitude: " << lon << std::endl;

  // If you tried with a string such as "Toulouse" -- a city where the
  // heart of OTB relies -- you should obtain something
  // like:
  //
  // \begin{verbatim}
  // Latitude: 43.6044
  // Longitude: 1.44295
  // \end{verbatim}

  return EXIT_SUCCESS;
}
