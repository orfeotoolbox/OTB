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


#include "otbImaginaryImageToComplexImageFilter.h"
#include "itkMacro.h"
#include "otbImageFileReader.h"
#include "otbImage.h"

#include "itkComplexToImaginaryImageFilter.h"
#include "otbStreamingCompareImageFilter.h"

int otbImaginaryImageToComplexImageFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];

  typedef double                   PixelType;
  typedef std::complex<PixelType>  ComplexPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension>        ImageType;
  typedef otb::Image<ComplexPixelType, Dimension> ComplexImageType;

  typedef otb::ImageFileReader<ImageType>  ReaderType;

  typedef otb::ImaginaryImageToComplexImageFilter<ImageType, ComplexImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  filter->SetInput(reader->GetOutput());
  filter->Update();

  typedef itk::ComplexToImaginaryImageFilter<ComplexImageType, ImageType> ImageFilterType;

  ImageFilterType::Pointer filterImaginaryPart = ImageFilterType::New();
  filterImaginaryPart->SetInput(filter->GetOutput());

  typedef otb::StreamingCompareImageFilter<ImageType>   CompareImageFilterType;
  CompareImageFilterType::Pointer compareImaginaryImageFilter = CompareImageFilterType::New();

  compareImaginaryImageFilter->SetInput1(filterImaginaryPart->GetOutput());
  compareImaginaryImageFilter->SetInput2(reader->GetOutput());
  compareImaginaryImageFilter->Update();

  if(compareImaginaryImageFilter->GetMSE() > 0.0000001)
    {
      std::cout << " Invalid ImaginaryPart ! MSE: "<< compareImaginaryImageFilter->GetMSE() << std::endl;
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
