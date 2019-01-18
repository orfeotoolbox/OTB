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

#include "otbShiftScaleVectorImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageList.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbStreamingStatisticsImageFilter.h"

int otbShiftScaleVectorImageFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char *        infname = argv[1];
  const char *        outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                                  InputPixelType;
  typedef float                                                                   OutputPixelType;
  typedef otb::VectorImage<InputPixelType, Dimension>                             InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension>                            OutputImageType;
  typedef otb::Image<InputPixelType, 2>                                            ImageType;
  typedef otb::ImageList<ImageType>                                               ImageListType;
  typedef otb::VectorImageToImageListFilter<InputImageType, ImageListType>        VI2ILFilterType;
  //Statistics estimator
  typedef otb::StreamingStatisticsImageFilter<ImageType>                          StreamingStatisticsImageFilterType;

  typedef otb::ShiftScaleVectorImageFilter<InputImageType, OutputImageType>       ShiftScaleVImageFilterType;

  typedef otb::ImageFileReader<InputImageType>                                    ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                                   WriterType;

  typedef itk::VariableLengthVector<InputPixelType>                               MeasurementType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  // Instantiating object
  ShiftScaleVImageFilterType::Pointer filter = ShiftScaleVImageFilterType::New();
  filter->SetInput(reader->GetOutput());

  // Build a Measurement Vector of mean
  MeasurementType mean;

  // Build a MeasurementVector of variance
  MeasurementType variance;

  int nbBands = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  ImageType::SizeType size;
  size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();

  //set the measurement vectors size
  mean.SetSize(nbBands);
  mean.Fill(0.);
  variance.SetSize(nbBands);
  variance.Fill(0.);

  //Splitting image into channels
  VI2ILFilterType::Pointer vi2ilFilter = VI2ILFilterType::New();
  vi2ilFilter->SetInput(reader->GetOutput());
  vi2ilFilter->GenerateOutputInformation();

  ImageListType::Iterator ilIt = vi2ilFilter->GetOutput()->Begin();

  unsigned int i = 0;
  //Iterate over each bands on the input vector image
  while (ilIt != vi2ilFilter->GetOutput()->End())
    {
    StreamingStatisticsImageFilterType::Pointer statsEstimator = StreamingStatisticsImageFilterType::New();
    statsEstimator->SetInput(ilIt.Get());
    statsEstimator->Update();

    //Compute mean over band number i
    mean[i] = statsEstimator->GetMean();
    //Compute variance over band i
    variance[i] = statsEstimator->GetVariance();
    ++ilIt;
    ++i;
    }

  std::cout<< "mean: " << mean << std::endl;
  std::cout<< "variance: " << variance << std::endl;

  filter->SetScale(variance);
  filter->SetShift(mean);

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
