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

#include "otbSarBrightnessFunction.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include <iostream>

int otbSarBrightnessFunction(int itkNotUsed(argc), char* argv[])
{

  const char * infname  = argv[1];
  const char * outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef otb::Image<PixelType, Dimension>                InputImageType;
  typedef InputImageType::IndexType                       IndexType;
  typedef otb::ImageFileReader<InputImageType>            ReaderType;
  typedef otb::SarBrightnessFunction<InputImageType>      FunctionType;

  /**Instantiation ofa Smart Pointer*/
  FunctionType::Pointer filter = FunctionType::New();
  ReaderType::Pointer   reader = ReaderType::New();

  std::ofstream outfile(outfname);

  /** Input Image*/
  reader->SetFileName(infname);
  reader->Update();

  /** Computing the density around a pixel  */
  filter->SetInputImage(reader->GetOutput());

  /** Test on some indexes and some physical coordinates*/
  InputImageType::SizeType size = reader->GetOutput()->GetRequestedRegion().GetSize();
  FunctionType::PointType  pDst;
  IndexType                index;

  index[0] = 0;
  index[1] = 0;
  outfile << "Sar Brightness value computed for the point : " << index << " is " << filter->EvaluateAtIndex(index) << std::endl;

  index[0] = static_cast<unsigned int>(size[0] / 2.);
  index[1] = static_cast<unsigned int>(size[1] / 4.);
  outfile << "Sar Brightness value computed for the point : " << index << " is " << filter->EvaluateAtIndex(index) << std::endl;

  pDst[0] = static_cast<unsigned int>(size[0] / 4.);
  pDst[1] = static_cast<unsigned int>(size[1] / 8.);
  outfile << "Sar Brightness value computed for the point : " <<  pDst << " is " << filter->Evaluate(pDst) << std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
