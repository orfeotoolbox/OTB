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
#include "otbMultivariateAlterationDetectorImageFilter.h"

typedef otb::VectorImage<unsigned short, 2> ImageType;
typedef otb::VectorImage<double, 2>         OutputImageType;
typedef otb::ImageFileReader<ImageType>    ReaderType;
typedef otb::ImageFileWriter<OutputImageType> WriterType;
typedef otb::MultivariateAlterationDetectorImageFilter<ImageType, OutputImageType> MADFilterType;


int otbMultivariateAlterationDetectorImageFilterNew(int argc, char* argv[])
{
  MADFilterType::Pointer  madFilter = MADFilterType::New();

  return EXIT_SUCCESS;
}


int otbMultivariateAlterationDetectorImageFilter(int argc, char* argv[])
{
  char * infname1 = argv[1];
  char * infname2 = argv[2];
  char * outfname = argv[3];

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(infname1);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(infname2);

  MADFilterType::Pointer  madFilter = MADFilterType::New();
  madFilter->SetInput1(reader1->GetOutput());
  madFilter->SetInput2(reader2->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(madFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  std::cout<<"V1: "<<std::endl;
  std::cout<<madFilter->GetV1()<<std::endl;

  std::cout<<"V2: "<<std::endl;
  std::cout<<madFilter->GetV2()<<std::endl;

  std::cout<<"Rho: "<<std::endl;
  std::cout<<madFilter->GetRho()<<std::endl;

  return EXIT_SUCCESS;
}
