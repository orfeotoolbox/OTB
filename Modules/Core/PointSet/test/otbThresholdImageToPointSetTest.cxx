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


#include "itkMacro.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbThresholdImageToPointSetFilter.h"

int otbThresholdImageToPointSetTest(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int    Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef itk::PointSet<InputPixelType, Dimension>                          PointSetType;
  typedef otb::ThresholdImageToPointSetFilter<InputImageType, PointSetType> FunctionType;
  typedef PointSetType::PointType          PointType;
  typedef FunctionType::InputPixelType     InputPixelType;
  typedef FunctionType::OutputPointSetType OutputPointSetType;

  InputPixelType LowerThreshold((InputPixelType)::atoi(argv[3]));
  InputPixelType UpperThreshold((InputPixelType)::atoi(argv[4]));

  ReaderType::Pointer   reader   = ReaderType::New();
  FunctionType::Pointer function = FunctionType::New();

  reader->SetFileName(inputFilename);

  function->SetInput(0, reader->GetOutput());
  function->SetLowerThreshold(LowerThreshold);
  function->SetUpperThreshold(UpperThreshold);
  function->Update();
  OutputPointSetType* pointList = function->GetOutput();

  std::ofstream file;
  file.open(outputFilename);

  unsigned long NbPoints = pointList->GetNumberOfPoints();
  file << "NbPoints : " << NbPoints << std::endl;

  PointType CoordPoint;

  // Initialization
  CoordPoint.Fill(0);

  for (unsigned long i = 0; i < NbPoints; ++i)
  {
    pointList->GetPoint(i, &CoordPoint);
    file << i + 1 << "/" << NbPoints << " : " << CoordPoint[0] << " , " << CoordPoint[1] << std::endl;
  }

  file.close();

  return EXIT_SUCCESS;
}
