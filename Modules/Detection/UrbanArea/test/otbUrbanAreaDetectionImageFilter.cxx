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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbUrbanAreaDetectionImageFilter.h"

int otbUrbanAreaDetectionImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputVectorImageType;
  typedef otb::Image<unsigned char, Dimension>   OutputImageType;

  typedef otb::ImageFileReader<InputVectorImageType> ReaderType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::UrbanAreaDetectionImageFilter<InputVectorImageType,
      OutputImageType> UrbanAreaDetectionFilterType;

  // Instantiating objects
  UrbanAreaDetectionFilterType::Pointer filter = UrbanAreaDetectionFilterType::New();
  ReaderType::Pointer                   reader = ReaderType::New();
  WriterType::Pointer                   writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  filter->SetInput(reader->GetOutput());
  filter->SetRedIndex(::atoi(argv[3]));
  filter->SetGreenIndex(::atoi(argv[4]));
  filter->SetNIRIndex(::atoi(argv[5]));
  filter->SetThresholdValue(::atof(argv[6]));
  filter->SetThresholdDensity(::atof(argv[7]));

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
