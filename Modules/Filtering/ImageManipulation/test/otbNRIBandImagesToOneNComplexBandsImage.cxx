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

#include "otbNRIBandImagesToOneNComplexBandsImage.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbNRIBandImagesToOneNComplexBandsImage(int itkNotUsed(argc), char * argv[])
{

  typedef double PixelType;
  typedef otb::VectorImage<PixelType,  2> InputImageType;
    
  typedef std::complex<double> OutputPixelType;
  typedef otb::VectorImage<OutputPixelType, 2> OutputImageType;
 
 
  typedef otb::NRIBandImagesToOneNComplexBandsImage<InputImageType, OutputImageType> FilterType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer readerA = ReaderType::New();
  ReaderType::Pointer readerB = ReaderType::New();
  ReaderType::Pointer readerC = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  WriterType::Pointer writer = WriterType::New();
  
  
  readerA->SetFileName(argv[1]);
  readerB->SetFileName(argv[2]);
  readerC->SetFileName(argv[3]);
  writer->SetFileName(argv[4]);

  filter->SetInput(0,readerA->GetOutput());
  filter->SetInput(1,readerB->GetOutput());
  filter->SetInput(2,readerC->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();



  return EXIT_SUCCESS;
}
