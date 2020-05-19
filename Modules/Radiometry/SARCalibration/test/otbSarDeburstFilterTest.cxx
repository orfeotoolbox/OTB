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

#include "otbSarDeburstImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

typedef otb::Image<unsigned short>            ImageType;
typedef otb::ImageFileReader<ImageType>       ReaderType;
typedef otb::ImageFileWriter<ImageType>       WriterType;
typedef otb::SarDeburstImageFilter<ImageType> DeburstFilterType;

int otbSarDeburstFilterTest(int argc, char* argv[])
{
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  bool onlyValidSamples = false;
  if (argc == 4)
  {
    onlyValidSamples = true;
  }

  DeburstFilterType::Pointer filter = DeburstFilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetOnlyValidSample(onlyValidSamples);

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  // check that there is now a single burst in data
  reader = ReaderType::New();
  reader->SetFileName(argv[2]);
  reader->UpdateOutputInformation();

  unsigned int nb_bursts = atoi(reader->GetOutput()->GetImageKeywordlist().GetMetadataByKey("support_data.geom.bursts.number").c_str());

  if (nb_bursts != 1)
  {
    std::cerr << "Error: more than 1 burst (" << nb_bursts << " bursts) found in output metadata." << std::endl;
  }
  else
  {
    std::cout << "Metadata have a single burst as expected." << std::endl;
  }

  return EXIT_SUCCESS;
}
