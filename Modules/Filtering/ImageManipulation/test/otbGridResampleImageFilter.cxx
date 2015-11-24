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
#include "itkRandomImageSource.h"
#include "itkIdentityTransform.h"
#include "itkTestingComparisonImageFilter.h"

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
  typedef itk::RandomImageSource<ImageType> RandomImageSourceType;

  RandomImageSourceType::Pointer randomImageSource = RandomImageSourceType::New();

  ImageType::SizeType size;
  size.Fill(100);

  randomImageSource->SetSize(size);
  randomImageSource->Update();

  filter->SetInput(randomImageSource->GetOutput());

  refFilter->SetInput(randomImageSource->GetOutput());

  typedef itk::IdentityTransform<double,2> IdentityTransformType;

  IdentityTransformType::Pointer idTransform = IdentityTransformType::New();
  refFilter->SetTransform(idTransform);

  ImageType::SpacingType spacing;
  spacing[0]=2.0;
  spacing[1]=-2.0;
  ImageType::PointType origin;
  origin[0]=10.;
  origin[1]=90.;
  ImageType::SizeType outSize;
  outSize.Fill(200);

  filter->SetOutputSize(outSize);
  filter->SetOutputOrigin(origin);
  filter->SetOutputSpacing(spacing);
  filter->Update();
  
  refFilter->SetSize(outSize);
  refFilter->SetOutputOrigin(origin);
  refFilter->SetOutputSpacing(spacing);
  refFilter->Update();

  typedef itk::Testing::ComparisonImageFilter<ImageType,ImageType> ComparisonFilterType;

  ComparisonFilterType::Pointer comparisonFilter = ComparisonFilterType::New();
  comparisonFilter->SetValidInput(refFilter->GetOutput());
  comparisonFilter->SetTestInput(filter->GetOutput());
  comparisonFilter->Update();

  unsigned int nbPixelsWithDiff = comparisonFilter->GetNumberOfPixelsWithDifferences();

  std::cout<<"Number of pixels with differences: "<<nbPixelsWithDiff<<std::endl;

  if(nbPixelsWithDiff)
    {
    std::cerr<<"Output of itk::GridResampleImageFilter does not match output of itk::ResampleImageFilter with same parameters"<<std::endl;

    return EXIT_FAILURE;
    }
  

  return EXIT_SUCCESS;
}
