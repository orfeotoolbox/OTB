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


/* Example usage:
./KullbackLeiblerDistanceChDet Input/GomaAvant.png Input/GomaApres.png Output/KLdistanceChDet.png 35
*/


#include "itkMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbKullbackLeiblerDistanceImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 5)
  {
    std::cerr << "Change detection through a Kullback-Leibler measure (which is a distance between local distributions)\n";
    std::cerr << "Kullback-Leibler measure is optimized by a Edgeworth series expansion\n";
    std::cerr << argv[0] << " imgAv imgAp imgResu winSize\n";
    return 1;
  }

  char* fileName1   = argv[1];
  char* fileName2   = argv[2];
  char* fileNameOut = argv[3];
  int   winSize     = atoi(argv[4]);

  const unsigned int Dimension = 2;
  using PixelType              = double;
  using OutputPixelType        = unsigned char;

  using ImageType       = otb::Image<PixelType, Dimension>;
  using OutputImageType = otb::Image<OutputPixelType, Dimension>;

  // The KullbackLeiblerDistanceImageFilter is templated over
  // the types of the two input images and the type of the generated change
  // image, in a similar way as the MeanRatioImageFilter. It is
  // the only line to be changed from the ratio of means change detection
  // example to perform a change detection through a distance between
  // distributions.
  using FilterType = otb::KullbackLeiblerDistanceImageFilter<ImageType, ImageType, ImageType>;

  // The different elements of the pipeline can now be instantiated. Follow the
  // ratio of means change detector example.
  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<OutputImageType>;

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(fileName1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(fileName2);

  // The only parameter for this change detector is the radius of
  // the window used for computing the cumulants.
  FilterType::Pointer filter = FilterType::New();
  filter->SetRadius((winSize - 1) / 2);

  // The pipeline is built by plugging all the elements together.
  filter->SetInput1(reader1->GetOutput());
  filter->SetInput2(reader2->GetOutput());

  using RescaleFilterType             = itk::RescaleIntensityImageFilter<ImageType, OutputImageType>;
  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

  rescaler->SetInput(filter->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(fileNameOut);
  writer->SetInput(rescaler->GetOutput());
  writer->Update();
}
