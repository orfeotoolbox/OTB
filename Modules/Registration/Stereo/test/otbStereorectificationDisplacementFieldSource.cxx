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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStereorectificationDisplacementFieldSource.h"

typedef itk::Vector<double,2>                               DisplacementType;
typedef otb::Image<DisplacementType>                         DisplacementFieldType;
typedef otb::Image<unsigned short>                          ImageType;
typedef otb::StereorectificationDisplacementFieldSource
   <ImageType,DisplacementFieldType>                         DisplacementFieldSourceType;
typedef otb::ImageFileReader<ImageType>                     ReaderType;
typedef otb::ImageFileWriter<DisplacementFieldType> WriterType;


int otbStereorectificationDisplacementFieldSource(int itkNotUsed(argc), char * argv[])
{
  char * infname1  = argv[1];
  char * infname2  = argv[2];
  char * outfname1 = argv[3];
  char * outfname2 = argv[4];
  double avgElev   = atof(argv[5]);
  double scale     = atof(argv[6]);
  double gridStep  = atoi(argv[7]);

  // Instantiation
  DisplacementFieldSourceType::Pointer dfSource = DisplacementFieldSourceType::New();

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(infname1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(infname2);

  dfSource->SetLeftImage(reader1->GetOutput());
  dfSource->SetRightImage(reader2->GetOutput());
  dfSource->SetGridStep(gridStep);
  dfSource->SetScale(scale);

  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(avgElev);

  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetInput(dfSource->GetLeftDisplacementFieldOutput());
  writer1->SetFileName(outfname1);
  writer1->Update();

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(dfSource->GetRightDisplacementFieldOutput());
  writer2->SetFileName(outfname2);
  writer2->Update();

  return EXIT_SUCCESS;
}
