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


#include "otbRealImageToComplexImageFilter.h"
#include "itkMacro.h"
#include "otbImageFileReader.h"
#include "otbImage.h"

#include "itkComplexToRealImageFilter.h"
#include "otbStreamingCompareImageFilter.h"

int otbRealImageToComplexImageFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];

  typedef double                   PixelType;
  typedef std::complex<PixelType>  ComplexPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension>        ImageType;
  typedef otb::Image<ComplexPixelType, Dimension> ComplexImageType;

  typedef otb::ImageFileReader<ImageType>  ReaderType;

  typedef otb::RealImageToComplexImageFilter<ImageType, ComplexImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  filter->SetInput(reader->GetOutput());
  filter->Update();

  typedef itk::ComplexToRealImageFilter<ComplexImageType, ImageType> ImageFilterType;

  ImageFilterType::Pointer filterRealPart = ImageFilterType::New();
  filterRealPart->SetInput(filter->GetOutput());

  typedef otb::StreamingCompareImageFilter<ImageType>   CompareImageFilterType;
  CompareImageFilterType::Pointer compareRealImageFilter = CompareImageFilterType::New();

  compareRealImageFilter->SetInput1(filterRealPart->GetOutput());
  compareRealImageFilter->SetInput2(reader->GetOutput());
  compareRealImageFilter->Update();

  if(compareRealImageFilter->GetMSE() > 0.0000001)
    {
      std::cout << " Invalid RealPart ! MSE: "<< compareRealImageFilter->GetMSE() << std::endl;
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
