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


#include "otbStreamingHistogramVectorImageFilter.h"
#include "otbVectorImage.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbObjectList.h"
#include "itkHistogram.h"

typedef otb::VectorImage<unsigned char>                                  VectorImageType;
typedef otb::StreamingHistogramVectorImageFilter<VectorImageType>        SHVIFType;
typedef itk::NumericTraits<VectorImageType::InternalPixelType>::RealType RealType;
typedef RealType                                                         MeasurementType;
typedef itk::Statistics::Histogram<MeasurementType>                      Histogram;
typedef otb::ObjectList<Histogram>                                       HistogramList;


int otbStreamingHistogramVectorImageFilterTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  // Allocate input image
  const unsigned int        nbComp = 2;
  VectorImageType::SizeType size;
  size.Fill(4);
  VectorImageType::IndexType idx;
  idx.Fill(0);
  VectorImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(idx);

  VectorImageType::Pointer image = VectorImageType::New();

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(nbComp);
  image->Allocate();

  typedef itk::ImageRegionIteratorWithIndex<VectorImageType> IteratorType;
  IteratorType                                               it(image, region);

  it.GoToBegin();

  VectorImageType::PixelType pixel(nbComp);
  VectorImageType::IndexType index;

  while (!it.IsAtEnd())
  {
    index    = it.GetIndex();
    pixel[0] = index[0];
    pixel[1] = index[1];

    it.Set(pixel);
    ++it;
  }
  // Histogram computation
  SHVIFType::Pointer SHVIFFilter = SHVIFType::New();

  SHVIFFilter->GetFilter()->SetInput(image);
  SHVIFType::FilterType::CountVectorType bins(nbComp);
  bins[0] = 2;
  bins[1] = 2;
  SHVIFFilter->GetFilter()->SetNumberOfBins(bins);

  VectorImageType::PixelType pixelMin(nbComp);
  pixelMin[0] = 0;
  pixelMin[1] = 0;
  VectorImageType::PixelType pixelMax(nbComp);
  pixelMax[0] = 3;
  pixelMax[1] = 3;

  SHVIFFilter->GetFilter()->SetHistogramMin(pixelMin);
  SHVIFFilter->GetFilter()->SetHistogramMax(pixelMax);

  SHVIFFilter->Update();

  HistogramList::Pointer histograms = SHVIFFilter->GetHistogramList();

  std::cout << "Histogram list size " << histograms->Size() << std::endl;
  unsigned int       channel = 0; // first channel
  Histogram::Pointer histogram(histograms->GetNthElement(channel));

  unsigned int histogramSize = histogram->Size();

  std::cout << "Histogram size " << histogramSize << std::endl;


  std::cout << "Histogram of the first component" << std::endl;

  // We expect to have 2 bins, each with a frequency of 8.
  const unsigned int expectedFrequency = 8;

  for (unsigned int bin = 0; bin < histogramSize; bin++)
  {
    if (histogram->GetFrequency(bin, channel) != expectedFrequency)
    {
      std::cerr << "Error in bin= " << bin << " channel = " << channel << std::endl;
      std::cerr << "Frequency was= " << histogram->GetFrequency(bin, channel) << " Instead of the expected " << expectedFrequency << std::endl;
      return EXIT_FAILURE;
    }
  }

  channel   = 1;
  histogram = histograms->GetNthElement(channel);

  histogramSize = histogram->Size();

  std::cout << "Histogram size " << histogramSize << std::endl;

  std::cout << "Histogram of the second component" << std::endl;

  for (unsigned int bin = 0; bin < histogramSize; bin++)
  {
    if (histogram->GetFrequency(bin, 0) != expectedFrequency)
    {
      std::cerr << "Error in bin= " << bin << " channel = " << channel << std::endl;
      std::cerr << "Frequency was= " << histogram->GetFrequency(bin, 0) << " Instead of the expected " << expectedFrequency << std::endl;
      return EXIT_FAILURE;
    }
  }
  // FIXME: Here we should also test the support of no data value
  return EXIT_SUCCESS;
}
