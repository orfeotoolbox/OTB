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




#include "itkMacro.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbHarrisImageToPointSetFilter.h"
#include <fstream>

int otbHarrisToPointSet(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  double SigmaD((double) ::atof(argv[3]));
  double SigmaI((double) ::atof(argv[4]));
  double AlphaValue((double) ::atof(argv[5]));

  typedef unsigned char PixelType;
  const unsigned int Dimension = 2;

  PixelType LowerThreshold((PixelType) ::atoi(argv[6]));
  PixelType UpperThreshold((PixelType) ::atoi(argv[7]));

  typedef otb::Image<PixelType,  Dimension>           ImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::HarrisImageToPointSetFilter<ImageType> FunctionType;
  typedef FunctionType::OutputPointSetType            OutputPointSetType;
  typedef OutputPointSetType::PointType               OutputPointType;

  ReaderType::Pointer   reader    = ReaderType::New();
  FunctionType::Pointer harris    = FunctionType::New();
  OutputPointType       CoordPoint;

  //Initialization
  CoordPoint.Fill(0);

  reader->SetFileName(inputFilename);

  harris->SetInput(0, reader->GetOutput());
  harris->SetSigmaD(SigmaD);
  harris->SetSigmaI(SigmaI);
  harris->SetAlpha(AlphaValue);
  harris->SetLowerThreshold(LowerThreshold);
  harris->SetUpperThreshold(UpperThreshold);
  OutputPointSetType * pointList = harris->GetOutput();

  harris->Update();

  std::cout << "Threshold Lower/Upper : " << harris->GetLowerThreshold() << "/" << harris->GetUpperThreshold() <<
  std::endl;
  std::ofstream file;
  file.open(outputFilename);

  unsigned long NbPoints  = pointList->GetNumberOfPoints();
  file << "NbPoints : " << NbPoints << std::endl;

  for (unsigned long i = 0; i < NbPoints; ++i)
    {
    pointList->GetPoint(i, &CoordPoint);
    file << i + 1 << " / " << NbPoints << " : ";
    file << CoordPoint[0] << " , " << CoordPoint[1] << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
