/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbSubsampleImageFilter.h"

int otbSubsampleImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName = argv[1];
  const char * outputFileName1 = argv[2];
  const char * outputFileName2 = argv[3];

  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  typedef otb::SubsampleImageFilter<ImageType, ImageType, otb::Wavelet::INVERSE>
  InvFilterType;
  InvFilterType::Pointer invFilter = InvFilterType::New();
  invFilter->SetInput(reader->GetOutput());

  itk::Index<Dimension> delta = {{2, 2}};
  invFilter->SetSubsampleFactor(delta);

  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetFileName(outputFileName1);
  writer1->SetInput(invFilter->GetOutput());
  writer1->Update();

  typedef otb::SubsampleImageFilter<ImageType, ImageType, otb::Wavelet::FORWARD>
  ForwardFilterType;
  ForwardFilterType::Pointer filter = ForwardFilterType::New();
  filter->SetInput(invFilter->GetOutput());
  filter->SetSubsampleFactor(invFilter->GetSubsampleFactor());

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName(outputFileName2);
  writer2->SetInput(filter->GetOutput());
  writer2->Update();

  return EXIT_SUCCESS;
}
