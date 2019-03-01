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


//  The following code is an implementation of a small OTB
//  program. It tests including header files and linking with OTB
//  libraries.

#include "otbImage.h"
#include <iostream>

int main()
{
  typedef otb::Image<unsigned short, 2> ImageType;

  ImageType::Pointer image = ImageType::New();

  std::cout << "OTB Hello World !" << std::endl;

  return EXIT_SUCCESS;
}

//  This code instantiates an image whose pixels are represented with
//  type \code{unsigned short}. The image is then constructed and assigned to a
//  \doxygen{itk}{SmartPointer}. Although later in the text we will discuss
//  \code{SmartPointer}'s in detail, for now think of it as a handle on an
//  instance of an object (see section \ref{sec:SmartPointers} for more
//  information).  The \doxygen{itk}{Image} class will be described in
//  Section~\ref{sec:ImageSection}.
