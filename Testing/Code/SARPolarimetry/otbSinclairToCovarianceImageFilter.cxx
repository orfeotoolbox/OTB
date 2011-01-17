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

#include "itkExceptionObject.h"
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSinclairImageFilter.h"

int otbSinclairToCovarianceImageFilter(int argc, char * argv[])
{
  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * inputFilename3  = argv[3];

  const char * outputFilename = argv[4];


  typedef std::complex <double> InputPixelType;
  typedef std::complex <double> OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>       InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::SinclairImageFilter<InputImageType, InputImageType,
                                      InputImageType, InputImageType,
                                      OutputImageType> FilterType;

  FilterType::Pointer sinclairToCovarianceImageFilter = FilterType::New();

  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  ReaderType::Pointer reader3 = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  reader3->SetFileName(inputFilename3);
  writer->SetFileName(outputFilename);

  sinclairToCovarianceImageFilter->SetInputHH(reader1->GetOutput());
  sinclairToCovarianceImageFilter->SetInputHV(reader2->GetOutput());
  sinclairToCovarianceImageFilter->SetInputVH(reader2->GetOutput());
  sinclairToCovarianceImageFilter->SetInputVV(reader3->GetOutput());
  writer->SetInput(sinclairToCovarianceImageFilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
