/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "itkMacro.h"
#include <cstdlib>
#include <cmath>

#include "otbRGBAPixelConverter.h"


int otbRGBAPixelConverter(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef unsigned int                                           PixelType0;
  typedef double                                                 PixelType1;
  typedef itk::RGBAPixel<unsigned int>                           PixelType2;
  typedef itk::RGBAPixel<double>                                 PixelType3;
  typedef itk::RGBPixel<double>                                  PixelType4;

  typedef otb::RGBAPixelConverter<PixelType0, PixelType0>        ConverterType0;
  typedef otb::RGBAPixelConverter<PixelType1, PixelType0>        ConverterType1;
  typedef otb::RGBAPixelConverter<PixelType0, PixelType3>        ConverterType2;
  typedef otb::RGBAPixelConverter<PixelType0, PixelType4>        ConverterType3;

  // Instantiating object
  ConverterType0::Pointer converter0 = ConverterType0::New();
  ConverterType1::Pointer converter1 = ConverterType1::New();
  ConverterType2::Pointer converter2 = ConverterType2::New();
  ConverterType3::Pointer converter3 = ConverterType3::New();

  PixelType2 pixel0;
  pixel0[0] = 125;
  pixel0[1] = 105;
  pixel0[2] = 145;
  pixel0[3] = 0;
  ConverterType0::OutputPixelType outputPixel0 = converter0->Convert(pixel0);
  std::cout << "outputPixel0: " << outputPixel0 << std::endl;
  if(outputPixel0 != 112)
    {
    itkGenericExceptionMacro(<< "RGBA<unsigned int> 2 unsigned int Failed");
    }

  PixelType3 pixel1;
  pixel1[0] = 125.0;
  pixel1[1] = 105.0;
  pixel1[2] = 145.0;
  pixel1[3] = 191.0;
  ConverterType1::OutputPixelType outputPixel1 = converter1->Convert(pixel1);
  std::cout << "outputPixel1: " << outputPixel1 << std::endl;
  if(outputPixel1 != 112)
    {
    itkGenericExceptionMacro(<< "RGBA<double> 2 unsigned int  Failed");
    }

  PixelType2 pixel2;
  pixel2[0] = 125;
  pixel2[1] = 105;
  pixel2[2] = 145;
  pixel2[3] = 0;
  ConverterType2::OutputPixelType outputPixel2 = converter2->Convert(pixel2);
  std::cout << "outputPixel2: " << outputPixel2 << std::endl;
  if(outputPixel2[0] != 125 || outputPixel2[1] != 105 || outputPixel2[2] != 145 || outputPixel2[3] != 0 )
    {
    itkGenericExceptionMacro(<< "RGBA<unsigned int> 2 RGBA<double>  Failed");
    }

  PixelType2 pixel3;
  pixel3[0] = 125;
  pixel3[1] = 105;
  pixel3[2] = 145;
  pixel3[3] = 0;
  ConverterType3::OutputPixelType outputPixel3 = converter3->Convert(pixel3);
  std::cout << "outputPixel3: " << outputPixel3 << std::endl;
  if(outputPixel3[0] != 125 || outputPixel3[1] != 105 || outputPixel3[2] != 145)
    {
    itkGenericExceptionMacro(<< "RGBA<unsigned int> 2 RGB<double>  Failed");
    }


  return EXIT_SUCCESS;
}
