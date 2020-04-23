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
#include "otbSoilDataBase.h"
#include <iostream>

int otbSoilDataBaseParseFile(int argc, char* argv[])
{
  if (argc < 5)
  {
    std::cout << "Usage: " << argv[0] << " soil_file wlmin wlmax wlstep wltest1 soilindextest1 refltest1 [wltest2 soilindextest2 refltest2 ...]" << '\n';
    return EXIT_FAILURE;
  }
  std::string sfn(argv[1]);

  otb::SoilDataBase sdb(sfn, 1000);
  auto              db = sdb.GetDB();

  for (unsigned int i = 5; i < static_cast<unsigned int>(argc); i += 3)
  {
    auto wltest    = std::stoi(argv[i]);
    auto sindex    = std::stoi(argv[i + 1]);
    auto refltest  = std::stod(argv[i + 2]);
    auto read_refl = db[sindex][wltest];
    if (fabs(refltest - read_refl) > 10e-5)
    {
      std::cout << "GetDB " << i << " " << refltest << " " << read_refl << "\n";
      return EXIT_FAILURE;
    }

    read_refl = sdb.GetReflectance(sindex, wltest);
    if (fabs(refltest - read_refl) > 10e-5)
    {
      std::cout << "GetRefl " << i << " " << refltest << " " << read_refl << "\n";
      return EXIT_FAILURE;
    }
  }

  for (unsigned int i = 5; i < static_cast<unsigned int>(argc); i += 3)
  {
    auto wltest    = std::stoi(argv[i]);
    auto sindex    = std::stoi(argv[i + 1]);
    auto refltest  = std::stod(argv[i + 2]);
    auto read_refl = db[sindex][wltest];

    read_refl = sdb.GetReflectance(sindex, wltest + 1);
    if (fabs(refltest - read_refl) > 10e-2)
    {
      std::cout << refltest << " " << read_refl << '\n';
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
