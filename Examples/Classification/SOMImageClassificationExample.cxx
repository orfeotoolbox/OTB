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

// In previous examples, we have used the
// \doxygen{otb}{SOMClassifier}, which uses the ITK classification
// framework. This good for compatibility with the ITK framework, but
// introduces the limitations of not being able to use streaming and
// being able to know at compilation time the number of bands of the
// image to be classified. In OTB we have avoided this limitation by
// developing the \doxygen{otb}{SOMImageClassificationFilter}. In
// this example we will illustrate its use. We start by including the
// appropriate header file.

#include "otbSOMImageClassificationFilter.h"
#include "otbImage.h"
#include "otbSOMMap.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* somfname = argv[2];
  const char* outfname = argv[3];

  // We will assume double precision input images and will also define
  // the type for the labeled pixels.

  const unsigned int     Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;
  // Our classifier will be generic enough to be able to process images
  // with any number of bands. We read the images as
  // \doxygen{otb}{VectorImage}s. The labeled image will be a scalar image.

  typedef otb::VectorImage<PixelType, Dimension>  ImageType;
  typedef otb::Image<LabeledPixelType, Dimension> LabeledImageType;
  // We can now define the type for the classifier filter, which is
  // templated over its input and output image types and the SOM type.

  typedef otb::SOMMap<ImageType::PixelType>                                          SOMMapType;
  typedef otb::SOMImageClassificationFilter<ImageType, LabeledImageType, SOMMapType> ClassificationFilterType;
  // And finally, we define the readers (for the input image and theSOM)
  // and the writer. Since the images,
  // to classify can be very big, we will use a streamed writer which
  // will trigger the streaming ability of the classifier.

  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileReader<SOMMapType>       SOMReaderType;
  typedef otb::ImageFileWriter<LabeledImageType> WriterType;
  // We instantiate the classifier and the reader objects and we set
  // the existing SOM obtained in a previous training step.

  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  SOMReaderType::Pointer somreader = SOMReaderType::New();
  somreader->SetFileName(somfname);
  somreader->Update();

  filter->SetMap(somreader->GetOutput());
  // We plug the pipeline and
  // trigger its execution by updating the output of the writer.

  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
  return EXIT_SUCCESS;
}
