/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbGridResampleImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkGaussianImageSource.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "itkIdentityTransform.h"
#include "otbDifferenceImageFilter.h"
#include "itkStreamingImageFilter.h"

#include "otbImageFileWriter.h"

int otbGridResampleImageFilter(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{

  // This test is only to check that filter builds with VectorImage
  // too  
  typedef otb::VectorImage<double> VectorImageType;
  typedef otb::GridResampleImageFilter<VectorImageType,VectorImageType> VectorFilterType;

  VectorFilterType::Pointer vectorFilter = VectorFilterType::New();
  
  typedef otb::Image<double> ImageType;
  typedef otb::GridResampleImageFilter<ImageType,ImageType> FilterType;
  
  FilterType::Pointer filter = FilterType::New();
  
  typedef itk::ResampleImageFilter<ImageType,ImageType> RefFilterType;

  RefFilterType::Pointer refFilter = RefFilterType::New();

  // We will resample a random image with GridResampleImageFilter and
  // ResampleImageFilter, and check that outputs are the same
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator   RandomGeneratorType;
  RandomGeneratorType::Pointer randomGenerator = RandomGeneratorType::GetInstance();

  ImageType::SizeType size;
  size.Fill(1000);
  
  ImageType::RegionType region;
  region.SetSize(size);

  ImageType::Pointer randomImage = ImageType::New();
  randomImage->SetRegions(region);
  randomImage->Allocate();
  typedef itk::ImageRegionIterator<ImageType> IteratorType;
  IteratorType iter(randomImage,region);
  for (iter.GoToBegin() ; !iter.IsAtEnd() ; ++iter)
    {
    iter.Set(randomGenerator->GetUniformVariate(0.0, 1.0) * 1000);
    }

  filter->SetInput(randomImage);

  refFilter->SetInput(randomImage);

  typedef itk::IdentityTransform<double,2> IdentityTransformType;

  IdentityTransformType::Pointer idTransform = IdentityTransformType::New();
  refFilter->SetTransform(idTransform);

  ImageType::SpacingType spacing;
  spacing[0]=0.12;
  spacing[1]=-0.19;
  ImageType::PointType origin;
  origin[0]=11;
  origin[1]=17;
  ImageType::SizeType outSize;
  outSize.Fill(103);

  filter->SetOutputSize(outSize);
  filter->SetOutputOrigin(origin);
  filter->SetOutputSpacing(spacing);

  refFilter->SetSize(outSize);
  refFilter->SetOutputOrigin(origin);
  refFilter->SetOutputSpacing(spacing);

  typedef otb::DifferenceImageFilter<ImageType,ImageType> ComparisonFilterType;
  typedef itk::StreamingImageFilter<ImageType,ImageType> StreamingFilterType;

  StreamingFilterType::Pointer streamingRef = StreamingFilterType::New();
  streamingRef->SetInput(refFilter->GetOutput());
  streamingRef->SetNumberOfStreamDivisions(10);

  StreamingFilterType::Pointer streaming = StreamingFilterType::New();
  streaming->SetInput(filter->GetOutput());
  streaming->SetNumberOfStreamDivisions(10);

  ComparisonFilterType::Pointer comparisonFilter = ComparisonFilterType::New();
  comparisonFilter->SetValidInput(streamingRef->GetOutput());
  comparisonFilter->SetTestInput(streaming->GetOutput());
  comparisonFilter->SetDifferenceThreshold(1e-9);
  comparisonFilter->Update();

  unsigned int nbPixelsWithDiff = comparisonFilter->GetNumberOfPixelsWithDifferences();

  std::cout<<"Number of pixels with differences: "<<nbPixelsWithDiff<<std::endl;

  if(nbPixelsWithDiff)
    {
    std::cerr<<"Output of itk::GridResampleImageFilter does not match output of itk::ResampleImageFilter with same parameters"<<std::endl;

    typedef otb::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(comparisonFilter->GetOutput());
    writer->SetFileName("otbGridResampleImageFilterTestOutput.tif");
    writer->Update();

    writer = WriterType::New();
    writer->SetInput(streamingRef->GetOutput());
    writer->SetFileName("otbGridResampleImageFilterTestOutput1.tif");
    writer->Update();
    
    writer = WriterType::New();
    writer->SetInput(streaming->GetOutput());
    writer->SetFileName("otbGridResampleImageFilterTestOutput2.tif");
    writer->Update();

    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
