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

#include "otbChangeInformationImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImage.h"

int otbChangeInformationImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename(argv[1]);

  typedef otb::Image<float, 2> ImageType;
  typedef otb::ChangeInformationImageFilter<ImageType> FilterType;
  typedef otb::ImageFileReader<ImageType>              ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  // try to set a different ProjRef
  std::string newProj("Fake ProjRef");
  filter->SetOutputMetaData<std::string>(otb::MetaDataKey::ProjectionRefKey, &newProj);
  // erase that choice
  filter->SetOutputMetaData<std::string>(otb::MetaDataKey::ProjectionRefKey, nullptr);
  // add a no data to the image
  std::vector<bool> flags;
  flags.push_back(true);
  std::vector<double> nodata;
  nodata.push_back(0.0);
  filter->SetOutputMetaData<std::vector<bool>>(otb::MetaDataKey::NoDataValueAvailable, &flags);
  filter->SetOutputMetaData<std::vector<double>>(otb::MetaDataKey::NoDataValue, &nodata);
  filter->UpdateOutputInformation();

  ImageType::Pointer outImage = filter->GetOutput();
  if (!outImage->GetProjectionRef().empty())
  {
    std::cout << "Projection is supposed to be removed but is still present !" << std::endl;
    return EXIT_FAILURE;
  }

  itk::MetaDataDictionary& dict = outImage->GetMetaDataDictionary();
  if (!dict.HasKey(otb::MetaDataKey::NoDataValueAvailable) || !dict.HasKey(otb::MetaDataKey::NoDataValue))
  {
    std::cout << "Missing no data metadata !" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
