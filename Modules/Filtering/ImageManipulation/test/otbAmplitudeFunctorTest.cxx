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

#include "otbAmplitudeFunctor.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "otbMath.h"

int otbAmplitudeFunctorTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double                   ScalarType;

  typedef otb::Function::AmplitudeFunctor<ScalarType>       FunctorType;
  typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                        RGBAPixelType;
  typedef VectorPixelType OutputPixelType;

  FunctorType funct;
  OutputPixelType output;
  std::vector<unsigned int> channels;
  ScalarType result;

  VectorPixelType vectorPixel;
  vectorPixel.SetSize(3);
  vectorPixel.SetElement(0, 1.0);
  vectorPixel.SetElement(1, 2.0);
  vectorPixel.SetElement(2, 3.0);

  // Test VectorPixelType
  for(unsigned int i = 0; i < 3; ++i)
    {
    for(unsigned int j = 0; j < 3; ++j)
      {
      channels.clear();
      channels.push_back(i);
      channels.push_back(j);
      funct.SetChannelList(channels);
      output = funct.operator ()(vectorPixel);
      result = std::sqrt(vectorPixel[i] * vectorPixel[i] + vectorPixel[j] * vectorPixel[j]);
      if( std::abs(static_cast<double>(result)-static_cast<double>(output[0])) > 0.0000001)
        {
          std::cout << "vectorPixelType Test VectorPixelType failed for channels " << i<< " and "
              << j << " !" << std::endl;
          return EXIT_FAILURE;
        }
      }
    }

  // Test RGBPixelType
  RGBPixelType  rgbPixel;
  rgbPixel.SetRed(1.0);
  rgbPixel.SetGreen(2.0);
  rgbPixel.SetBlue(3.0);
  for(unsigned int i = 0; i < 3; ++i)
    {
    for(unsigned int j = 0; j < 3; ++j)
      {
      channels.clear();
      channels.push_back(i);
      channels.push_back(j);
      funct.SetChannelList(channels);
      output = funct.operator ()(rgbPixel);
      result = std::sqrt(rgbPixel[i] * rgbPixel[i] + rgbPixel[j] * rgbPixel[j]);
      if( std::abs(static_cast<double>(result)-static_cast<double>(output[0])) > 0.0000001)
        {
          std::cout << "vectorPixelType Test RGBPixelType failed for channels " << i<< " and "
              << j << " !" << std::endl;
          return EXIT_FAILURE;
        }
      }
    }




  // Test RGBPAixelType
  RGBAPixelType rgbaPixel;
  rgbaPixel.SetRed(1.0);
  rgbaPixel.SetGreen(2.0);
  rgbaPixel.SetBlue(3.0);
  rgbaPixel.SetAlpha(4.0);

  for(unsigned int i = 0; i < 4; ++i)
    {
    for(unsigned int j = 0; j < 4; ++j)
      {
      channels.clear();
      channels.push_back(i);
      channels.push_back(j);
      funct.SetChannelList(channels);
      output = funct.operator ()(rgbaPixel);
      result = std::sqrt(rgbaPixel[i] * rgbaPixel[i] + rgbaPixel[j] * rgbaPixel[j]);
      if( std::abs(static_cast<double>(result)-static_cast<double>(output[0])) > 0.0000001)
        {
          std::cout << "vectorPixelType Test RGBAPixelType failed for channels " << i<< " and "
              << j << " !" << std::endl;
          return EXIT_FAILURE;
        }
      }
    }

  return EXIT_SUCCESS;
}
