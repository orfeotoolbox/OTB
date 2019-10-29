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

#if defined(_MSC_VER)
#pragma warning(disable : 4786)
#endif

#include "otbWrapperCommandLineLauncher.h"


int otbWrapperCommandLineLauncherTest(int argc, char* argv[])
{
  typedef otb::Wrapper::CommandLineLauncher LauncherType;
  LauncherType::Pointer                     launcher = LauncherType::New();

  std::vector<std::string> cmdVector;
  if (argc > 2)
  {
    for (int i = 1; i < argc; i++)
    {
      cmdVector.push_back(std::string(argv[i]));
    }
  }

  if (launcher->Load(cmdVector) == true)
  {
    if (launcher->ExecuteAndWriteOutput() == false)
    {
      return EXIT_FAILURE;
    }
  }
  else
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
