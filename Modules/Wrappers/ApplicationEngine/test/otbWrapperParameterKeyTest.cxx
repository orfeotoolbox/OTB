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

#include "otbWrapperParameterKey.h"

int otbWrapperParameterKey(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{

  std::string theKey = "parent.current.child";

  otb::Wrapper::ParameterKey pKey;
  pKey.SetKey(theKey);

  if (pKey.GetKey() != theKey)
  {
    std::cout << "Error in Set/GetKey method. Waiting for \"" << theKey << "\" receiving " << pKey.GetKey() << "." << std::endl;
    return EXIT_FAILURE;
  }

  otb::Wrapper::ParameterKey pKey1(theKey);
  if (pKey1.GetKey() != theKey)
  {
    std::cout << "Error in constructor method. Waiting for \"" << theKey << "\" receiving " << pKey1.GetKey() << "." << std::endl;
    return EXIT_FAILURE;
  }

  otb::Wrapper::ParameterKey pKey2(theKey);
  if (pKey2.GetKey() != theKey)
  {
    std::cout << "Error in constructor method. Waiting for \"" << theKey << "\" receiving " << pKey2.GetKey() << "." << std::endl;
    return EXIT_FAILURE;
  }


  if (pKey.GetLastElement() != "child")
  {
    std::cout << "Error in GetLastElement method. Waiting for \"child\" receiving " << pKey.GetLastElement() << "." << std::endl;
    return EXIT_FAILURE;
  }

  if (pKey.GetFirstElement() != "parent")
  {
    std::cout << "Error in GetFirstElement method. Waiting for \"parent\" receiving " << pKey.GetFirstElement() << "." << std::endl;
    return EXIT_FAILURE;
  }


  if (pKey.GetRoot() != "parent.current")
  {
    std::cout << "Error in GetRoot method. Waiting for \"parent.child\" receiving " << pKey.GetRoot() << "." << std::endl;
    return EXIT_FAILURE;
  }


  std::vector<std::string> tmp = pKey.Split();
  if (tmp.size() != 3)
  {
    std::cout << "Error in Split method. Waiting for a 3 size vector, receiving a vector of size " << tmp.size() << "." << std::endl;
    return EXIT_FAILURE;
  }

  if (tmp[0] != "parent" || tmp[1] != "current" || tmp[2] != "child")
  {
    std::cout << "Error in Split method. Waiting for [parent, current, child], receiving :[" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << "]" << std::endl;
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
