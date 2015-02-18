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



#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "otbComplexToIntensityImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "otbStreamingCompareImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"


int otbCompareWritingComplexImageTest(int argc, char* argv[])
{
  if (argc < 7)
    {
    std::cout << argv[0] << "<inputImage> <outputImage> <startX> <startY> <sizeX> <sizeY>"  << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];
  unsigned int startX = (unsigned int)(::atoi(argv[3]));
  unsigned int startY = (unsigned int)(::atoi(argv[4]));
  unsigned int sizeX = (unsigned int)(::atoi(argv[5]));
  unsigned int sizeY = (unsigned int)(::atoi(argv[6]));

  typedef std::complex<float> ComplexPixelType;
  typedef float               PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<ComplexPixelType,  Dimension> ComplexImageType;
  typedef otb::Image<PixelType,  Dimension>        ImageType;
  typedef otb::ImageFileReader<ComplexImageType>   ReaderType;
  typedef otb::ImageFileWriter<ComplexImageType>   WriterType;
  typedef otb::ExtractROI<ComplexPixelType, ComplexPixelType>  ExtractROIFilterType;
  typedef otb::StreamingCompareImageFilter<ImageType>          CompareFilterType;
  typedef itk::ComplexToRealImageFilter<ComplexImageType, ImageType>     RealExtractorType;
  typedef itk::ComplexToImaginaryImageFilter<ComplexImageType, ImageType> ImaginaryExtractorType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);


  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  extractROIFilter->SetStartX(startX);
  extractROIFilter->SetStartY(startY);
  extractROIFilter->SetSizeX(sizeX);
  extractROIFilter->SetSizeY(sizeY);
  extractROIFilter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(extractROIFilter->GetOutput());
  writer->Update();

  std::cout << "Write " << std::endl;

  ReaderType::Pointer readerAfterWrite = ReaderType::New();
  readerAfterWrite->SetFileName(outputFilename);
  readerAfterWrite->Update();

  /* Compare Real Part */
  RealExtractorType::Pointer realExtractor = RealExtractorType::New();
  RealExtractorType::Pointer realExtractorAfterWrite = RealExtractorType::New();

  realExtractor->SetInput(extractROIFilter->GetOutput());
  realExtractorAfterWrite->SetInput(readerAfterWrite->GetOutput());

  CompareFilterType::Pointer compareRealPartFilter = CompareFilterType::New();
  compareRealPartFilter->SetInput1(realExtractor->GetOutput());
  compareRealPartFilter->SetInput2(realExtractorAfterWrite->GetOutput());
  compareRealPartFilter->Update();

  /* Compare Imaginary Part */
  ImaginaryExtractorType::Pointer imagExtractor = ImaginaryExtractorType::New();
  ImaginaryExtractorType::Pointer imagExtractorAfterWrite = ImaginaryExtractorType::New();

  imagExtractor->SetInput(extractROIFilter->GetOutput());
  imagExtractorAfterWrite->SetInput(readerAfterWrite->GetOutput());

  CompareFilterType::Pointer compareImagPartFilter = CompareFilterType::New();
  compareImagPartFilter->SetInput1(imagExtractor->GetOutput());
  compareImagPartFilter->SetInput2(imagExtractorAfterWrite->GetOutput());
  compareImagPartFilter->Update();

  std::cout << " Real Part comparison : " << compareRealPartFilter->GetMSE()<< std::endl;
  std::cout << " Imaginary Part comparison : " << compareImagPartFilter->GetMSE()<< std::endl;


  if( compareRealPartFilter->GetMSE() > 0.0 )
    {
    std::cout << " Real part not the same " << std::endl;
    return EXIT_FAILURE;
    }

  if( compareImagPartFilter->GetMSE() > 0.0 )
    {
    std::cout << " Imaginary part not the same" << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}
