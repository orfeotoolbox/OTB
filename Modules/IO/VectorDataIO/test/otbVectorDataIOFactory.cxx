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


#include "otbVectorDataIOFactory.h"
#include "otbVectorData.h"

int otbVectorDataIOFactory(int itkNotUsed(argc), char* argv[])
{
  typedef otb::VectorDataIOFactory                              VectorData1IOFactoryType;
  typedef VectorData1IOFactoryType::VectorDataIOBasePointerType VectorData1IOBasePointerType;

  VectorData1IOBasePointerType io1 = VectorData1IOFactoryType::CreateVectorDataIO(argv[1], VectorData1IOFactoryType::ReadMode);
  std::cout << io1 << std::endl;

  return EXIT_SUCCESS;
}
