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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMaximumAutocorrelationFactorImageFilter.h"

typedef otb::VectorImage<unsigned short, 2> ImageType;
typedef otb::VectorImage<double, 2>         OutputImageType;
typedef otb::ImageFileReader<ImageType>    ReaderType;
typedef otb::ImageFileWriter<OutputImageType> WriterType;
typedef otb::MaximumAutocorrelationFactorImageFilter<ImageType, OutputImageType> MADFilterType;

int otbMaximumAutocorrelationFactorImageFilterNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  MADFilterType::Pointer  madFilter = MADFilterType::New();

  return EXIT_SUCCESS;
}


int otbMaximumAutocorrelationFactorImageFilter(int itkNotUsed(argc), char* argv[])
{
  char * infname = argv[1];
  char * outfname = argv[2];

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  MADFilterType::Pointer  madFilter = MADFilterType::New();
  madFilter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(madFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  std::cout<<"V: "<<std::endl;
  std::cout<<madFilter->GetV()<<std::endl;

  std::cout<<"Auto-correlation: "<<std::endl;
  std::cout<<madFilter->GetAutoCorrelation()<<std::endl;

  return EXIT_SUCCESS;
}
