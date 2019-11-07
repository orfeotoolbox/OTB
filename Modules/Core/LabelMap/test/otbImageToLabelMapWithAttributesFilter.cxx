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
#include "otbVectorImage.h"
#include "otbImageToLabelMapWithAttributesFilter.h"
#include "otbImageFileReader.h"

int otbImageToLabelMapWithAttributesFilter(int itkNotUsed(argc), char* argv[])
{
  const char* infname = argv[1];
  const char* lfname  = argv[2];

  // Convenient typedefs
  typedef otb::VectorImage<double, 2> ImageType;
  typedef otb::Image<unsigned int, 2> LabeledImageType;
  typedef otb::AttributesMapLabelObjectWithClassLabel<double, 2, double, double> LabelObjectType;

  typedef otb::ImageToLabelMapWithAttributesFilter<ImageType, LabeledImageType, unsigned int, LabelObjectType> FilterType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileReader<LabeledImageType> LabeledReaderType;

  // SmartPointer instantiation
  FilterType::Pointer        filter        = FilterType::New();
  ReaderType::Pointer        reader        = ReaderType::New();
  LabeledReaderType::Pointer labeledReader = LabeledReaderType::New();

  // Inputs
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  labeledReader->SetFileName(lfname);
  labeledReader->UpdateOutputInformation();

  // Filter
  filter->SetInput(reader->GetOutput());
  filter->SetLabeledImage(labeledReader->GetOutput());
  filter->Update();


  // tests
  std::vector<std::string> features = filter->GetOutput()->GetNthLabelObject(0)->GetAvailableAttributes();

  for (std::vector<std::string>::const_iterator fit = features.begin(); fit != features.end(); ++fit)
  {
    std::cout << "Label " << *fit << std::endl;
  }


  return EXIT_SUCCESS;
}
