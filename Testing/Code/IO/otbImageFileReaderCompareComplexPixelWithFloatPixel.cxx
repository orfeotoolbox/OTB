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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImage.h"
#include "itkExceptionObject.h"
#include "otbImageFileReader.h"
#include "otbExtractROI.h"
#include "otbComplexToIntensityImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "otbStreamingCompareImageFilter.h"

int otbImageFileReaderCompareComplexPixelWithFloatPixelTest(int argc, char* argv[])
{
  if (argc < 6)
    {
    std::cout << argv[0] << "<inputImage> <startX> <startY> <sizeX> <sizeY>"  << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  unsigned int startX = (unsigned int)(::atoi(argv[2]));
  unsigned int startY = (unsigned int)(::atoi(argv[3]));
  unsigned int sizeX = (unsigned int)(::atoi(argv[4]));
  unsigned int sizeY = (unsigned int)(::atoi(argv[5]));

  typedef std::complex<float> ComplexPixelType;
  typedef float               PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType,  Dimension> ImageType;
  typedef otb::Image<ComplexPixelType,  Dimension> ComplexImageType;

  typedef otb::ImageFileReader<ImageType>  FloatReaderType;
  typedef otb::ImageFileReader<ComplexImageType>  ComplexReaderType;

  ComplexReaderType::Pointer complexReader = ComplexReaderType::New();
  FloatReaderType::Pointer floatReader = FloatReaderType::New();

  complexReader->SetFileName(inputFilename);
  floatReader->SetFileName(inputFilename);

  typedef otb::ExtractROI<PixelType, PixelType>  FloatExtractROIFilterType;
  typedef otb::ExtractROI<ComplexPixelType, ComplexPixelType>  ComplexExtractROIFilterType;

  ComplexExtractROIFilterType::Pointer complexExtractROIFilter = ComplexExtractROIFilterType::New();
  FloatExtractROIFilterType::Pointer floatExtractROIFilter = FloatExtractROIFilterType::New();

  floatExtractROIFilter->SetStartX(startX);
  floatExtractROIFilter->SetStartY(startY);
  floatExtractROIFilter->SetSizeX(sizeX);
  floatExtractROIFilter->SetSizeY(sizeY);
  floatExtractROIFilter->SetInput(floatReader->GetOutput());

  complexExtractROIFilter->SetStartX(startX);
  complexExtractROIFilter->SetStartY(startY);
  complexExtractROIFilter->SetSizeX(sizeX);
  complexExtractROIFilter->SetSizeY(sizeY);
  complexExtractROIFilter->SetInput(complexReader->GetOutput());

  typedef itk::ComplexToRealImageFilter<ComplexImageType, ImageType> RealPartFilterType;
  RealPartFilterType::Pointer realPart = RealPartFilterType::New();
  realPart->SetInput(complexExtractROIFilter->GetOutput());

  typedef otb::StreamingCompareImageFilter<ImageType> CompareFilterType;
  CompareFilterType::Pointer compareWithRealPartFilter = CompareFilterType::New();
  compareWithRealPartFilter->SetInput1(floatExtractROIFilter->GetOutput());
  compareWithRealPartFilter->SetInput2(realPart->GetOutput());
  compareWithRealPartFilter->Update();

  typedef itk::ComplexToImaginaryImageFilter<ComplexImageType, ImageType> ImagPartFilterType;
  ImagPartFilterType::Pointer imagPart = ImagPartFilterType::New();
  imagPart->SetInput(complexExtractROIFilter->GetOutput());

  CompareFilterType::Pointer compareWithImagPartFilter = CompareFilterType::New();
  compareWithImagPartFilter->SetInput1(floatExtractROIFilter->GetOutput());
  compareWithImagPartFilter->SetInput2(imagPart->GetOutput());
  compareWithImagPartFilter->Update();

  if( compareWithRealPartFilter->GetMSE() > 0.0 ||
      compareWithImagPartFilter->GetMSE() < 0.0000001 )
    {
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
