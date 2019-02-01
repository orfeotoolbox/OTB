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

#include "otbImageClassificationFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMachineLearningModelFactory.h"

const unsigned int Dimension = 2;
typedef double PixelType;
typedef unsigned short LabeledPixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::Image<LabeledPixelType, Dimension> LabeledImageType;
typedef otb::ImageClassificationFilter<ImageType, LabeledImageType> ClassificationFilterType;
typedef ClassificationFilterType::ModelType ModelType;
typedef ClassificationFilterType::ValueType ValueType;
typedef ClassificationFilterType::LabelType LabelType;
typedef otb::MachineLearningModelFactory<ValueType, LabelType> MachineLearningModelFactoryType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::ImageFileWriter<LabeledImageType> WriterType;


int otbImageClassificationFilterLoadModel(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * modelfname = argv[2];

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  ModelType::Pointer model;

  model = MachineLearningModelFactoryType::CreateMachineLearningModel(modelfname,
                                                                      MachineLearningModelFactoryType::ReadMode);

  if (model.IsNull())
    {
    std::cerr << "Unable to create a model from " << modelfname << std::endl;
    return EXIT_FAILURE;
    }

  model->Load(modelfname);
  return EXIT_SUCCESS;
}

int otbImageClassificationFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * modelfname = argv[2];
  const char * outfname = argv[3];

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  ModelType::Pointer model;

  model = MachineLearningModelFactoryType::CreateMachineLearningModel(modelfname,
                                                                      MachineLearningModelFactoryType::ReadMode);

  if (model.IsNull())
  {
    std::cerr << "Unable to create a model from " << modelfname << std::endl;
    return EXIT_FAILURE;
  }

  model->Load(modelfname);

  filter->SetModel(model);
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
