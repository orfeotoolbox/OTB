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



#include "itkMacro.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "otbMultiChannelExtractROI.h"

int otbImageFileReaderWithComplexPixelTest(int argc, char* argv[])
{
  if (argc < 7)
    {
    std::cout << argv[0] << "<inputImage> <outputImage> <startX> <startY> <sizeX> <sizeY>"  << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  unsigned int startX = (unsigned int)(::atoi(argv[3]));
  unsigned int startY = (unsigned int)(::atoi(argv[4]));
  unsigned int sizeX = (unsigned int)(::atoi(argv[5]));
  unsigned int sizeY = (unsigned int)(::atoi(argv[6]));

  typedef std::complex<float> PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType,  Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer complexReader = ReaderType::New();

  complexReader->SetFileName(inputFilename);

  typedef otb::ExtractROI<PixelType, PixelType>  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetStartX(startX);
  extractROIFilter->SetStartY(startY);
  extractROIFilter->SetSizeX(sizeX);
  extractROIFilter->SetSizeY(sizeY);
  extractROIFilter->SetInput(complexReader->GetOutput());

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outputFilename);
  writer->SetInput(extractROIFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbVectorImageFileReaderWithComplexPixelTest(int argc, char* argv[])
{
  if (argc < 7)
    {
    std::cout << argv[0] << "<inputImage> <outputImage> <startX> <startY> <sizeX> <sizeY>"  << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  unsigned int startX = (unsigned int)(::atoi(argv[3]));
  unsigned int startY = (unsigned int)(::atoi(argv[4]));
  unsigned int sizeX = (unsigned int)(::atoi(argv[5]));
  unsigned int sizeY = (unsigned int)(::atoi(argv[6]));

  typedef std::complex<float>                            PixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<PixelType, Dimension>         CplVectorImageType;

  typedef otb::ImageFileReader<CplVectorImageType>  ReaderType;
  typedef otb::ImageFileWriter<CplVectorImageType>  WriterType;

  typedef otb::MultiChannelExtractROI<CplVectorImageType::InternalPixelType,
      CplVectorImageType::InternalPixelType>               ExtractROIFilterType;


  ReaderType::Pointer complexReader = ReaderType::New();

  complexReader->SetFileName(inputFilename);

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetStartX(startX);
  extractROIFilter->SetStartY(startY);
  extractROIFilter->SetSizeX(sizeX);
  extractROIFilter->SetSizeY(sizeY);
  extractROIFilter->SetInput(complexReader->GetOutput());

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outputFilename);
  writer->SetInput(extractROIFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
