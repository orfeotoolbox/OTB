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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbRadiometricMomentsImageFilter.h"

int otbRadiometricMomentsImageFilterNew(int argc, char * argv[])
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  /** Typedefs */
  typedef otb::Image<InputPixelType,  Dimension>       ImageType;
  typedef otb::VectorImage<InputPixelType,  Dimension> VectorImageType;

  typedef otb::RadiometricMomentsImageFilter<ImageType, VectorImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}

int otbRadiometricMomentsImageFilter(int argc, char * argv[])
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  // Typedefs
  typedef otb::Image<InputPixelType,  Dimension>         ImageType;
  typedef otb::ImageFileReader<ImageType>                ReaderType;
  typedef otb::VectorImage<InputPixelType,  Dimension>   VectorImageType;
  typedef otb::ImageFileWriter<VectorImageType> WriterType;
  typedef otb::RadiometricMomentsImageFilter<ImageType, VectorImageType> FilterType;

  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();

  filter->SetInput(reader->GetOutput());
  filter->SetRadius(atoi(argv[3]));
  filter->SetNumberOfThreads(1);

  writer->SetFileName(argv[2]);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

