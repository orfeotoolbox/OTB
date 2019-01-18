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

#include <iostream>

#include "otbImageFileReader.h"

#include "otbLocalHoughFilter.h"

#include <iostream>
#include <fstream>

int otbLocalHough(int itkNotUsed(argc), char* argv[])
{
  const char * inputFilename  = argv[1];
  const char * outfname  = argv[2];
  unsigned int RadiusX((unsigned int) ::atoi(argv[3]));
  unsigned int RadiusY((unsigned int) ::atoi(argv[4]));
  unsigned int NumberOfLines((unsigned int) ::atoi(argv[5]));

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;

  typedef otb::LocalHoughFilter<InputImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusY;

  typedef otb::LineSpatialObjectList LinesListType;
  LinesListType::Pointer list = LinesListType::New();

  filter->SetRadius(Radius);
  filter->SetNumberOfLines(NumberOfLines);

  filter->SetInput(reader->GetOutput());
  filter->Update();

  list = filter->GetOutput();

  LinesListType::const_iterator itList;

  std::ofstream outfile(outfname);
  outfile << "size of the Line list " << list->size() << std::endl;

  for (itList = list->begin(); itList != list->end(); itList++)
    outfile << (*itList)->GetPoints()[0].GetPosition()  << " \t" << (*itList)->GetPoints()[1].GetPosition()   <<
    std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
