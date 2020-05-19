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

#include "otbConvexOrConcaveClassificationFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbConvexOrConcaveClassificationFilter(int itkNotUsed(argc), char* argv[])
{
  const char*  infname       = argv[1];
  const char*  levelingfname = argv[2];
  const char*  outfname      = argv[3];
  const double sigma         = atof(argv[4]);

  typedef otb::Image<double, 2>        InputImageType;
  typedef otb::Image<unsigned char, 2> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::ConvexOrConcaveClassificationFilter<InputImageType, OutputImageType> ClassificationFilterType;

  // Instantiating objects
  ClassificationFilterType::Pointer filter         = ClassificationFilterType::New();
  ReaderType::Pointer               reader         = ReaderType::New();
  ReaderType::Pointer               levelingReader = ReaderType::New();
  WriterType::Pointer               writer         = WriterType::New();

  reader->SetFileName(infname);
  levelingReader->SetFileName(levelingfname);

  filter->SetInput(reader->GetOutput());
  filter->SetInputLeveling(levelingReader->GetOutput());
  filter->SetConvexLabel(255);
  filter->SetConcaveLabel(128);
  filter->SetFlatLabel(0);
  filter->SetSigma(sigma);

  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
