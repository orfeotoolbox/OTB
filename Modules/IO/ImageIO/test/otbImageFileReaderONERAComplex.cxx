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



#include <iostream>
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"

int otbImageFileReaderONERAComplex(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputRealFilename = argv[2];
  const char * outputImagFilename = argv[3];

  typedef std::complex<float> InputPixelType;
  typedef float               OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer Reader = ReaderType::New();

  Reader->SetFileName(inputFilename);

  typedef otb::ExtractROI<InputPixelType,
      InputPixelType>  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetStartX(10);
  extractROIFilter->SetStartY(10);
  extractROIFilter->SetSizeX(100);
  extractROIFilter->SetSizeY(100);
  extractROIFilter->SetInput(Reader->GetOutput());

  typedef itk::ComplexToRealImageFilter<
      InputImageType, OutputImageType> RealFilterType;
  typedef itk::ComplexToImaginaryImageFilter<
      InputImageType, OutputImageType> ImagFilterType;

  RealFilterType::Pointer realFilter = RealFilterType::New();
  realFilter->SetInput(extractROIFilter->GetOutput());
  ImagFilterType::Pointer imagFilter = ImagFilterType::New();
  imagFilter->SetInput(extractROIFilter->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputRealFilename);
  writer->SetInput(realFilter->GetOutput());
  writer->Update();
  writer = WriterType::New();
  writer->SetFileName(outputImagFilename);
  writer->SetInput(imagFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
