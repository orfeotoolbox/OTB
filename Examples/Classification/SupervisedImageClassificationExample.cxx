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


// In OTB, a generic streamed filter called \doxygen{otb}{ImageClassificationFilter}
// is available to classify any input multi-channel image according to an input
// classification model file. This filter is generic because it works with any
// classification model type (SVM, KNN, Artificial Neural Network,...) generated
// within the OTB generic Machine Learning framework based on OpenCV (\cite{opencv_library}).
// The input model file is smartly parsed according to its content in order to
// identify which learning method was used to generate it. Once the classification
// method and model are known, the input image can be classified. More details are
// given in subsections \ref{ssec:LearningFromSamples} and \ref{ssec:LearningFromImages}
// to generate a classification model either from samples or from images.
// In this example we will illustrate its use. We start by including the
// appropriate header files.

#include "otbMachineLearningModelFactory.h"
#include "otbImageClassificationFilter.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int itkNotUsed(argc), char* argv[])
{
  const char* infname    = argv[1];
  const char* modelfname = argv[2];
  const char* outfname   = argv[3];

  // We will assume double precision input images and will also define
  // the type for the labeled pixels.

  const unsigned int Dimension = 2;
  using PixelType              = double;
  using LabeledPixelType       = unsigned short;
  // Our classifier is generic enough to be able to process images
  // with any number of bands. We read the input image as a
  // \doxygen{otb}{VectorImage}. The labeled image will be a scalar image.
  using ImageType        = otb::VectorImage<PixelType, Dimension>;
  using LabeledImageType = otb::Image<LabeledPixelType, Dimension>;

  // We can now define the type for the classifier filter, which is
  // templated over its input and output image types.
  using ClassificationFilterType = otb::ImageClassificationFilter<ImageType, LabeledImageType>;
  using ModelType                = ClassificationFilterType::ModelType;

  // Moreover, it is necessary to define a \doxygen{otb}{MachineLearningModelFactory}
  // which is templated over its input and output pixel types. This factory is used
  // to parse the input model file and to define which classification method to use.
  using MachineLearningModelFactoryType = otb::MachineLearningModelFactory<PixelType, LabeledPixelType>;

  // And finally, we define the reader and the writer. Since the images
  // to classify can be very big, we will use a streamed writer which
  // will trigger the streaming ability of the classifier.
  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<LabeledImageType>;

  // We instantiate the classifier and the reader objects and we set
  // the existing model obtained in a previous training step.
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  // The input model file is parsed according to its content and the generated model
  // is then loaded within the \doxygen{otb}{ImageClassificationFilter}.
  ModelType::Pointer model;
  model = MachineLearningModelFactoryType::CreateMachineLearningModel(modelfname, MachineLearningModelFactoryType::ReadMode);
  model->Load(modelfname);

  filter->SetModel(model);

  // We plug the pipeline and
  // trigger its execution by updating the output of the writer.

  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
  return EXIT_SUCCESS;
}
