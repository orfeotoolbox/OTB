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

#include "itkShiftScaleImageFilter.h"
#include "otbStreamingCompareImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImage.h"
#include <fstream>
#include "otbStreamingTraits.h"

int otbStreamingCompareImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;

  typedef otb::Image<PixelType, Dimension>               ImageType;
  typedef otb::StreamingCompareImageFilter<ImageType> StreamingCompareImageFilterType;

  // Instantiating object
  StreamingCompareImageFilterType::Pointer filter = StreamingCompareImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}

int otbStreamingCompareImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::Image<PixelType, Dimension>               ImageType;
  typedef otb::ImageFileReader<ImageType>                ReaderType;
  typedef otb::StreamingCompareImageFilter<ImageType> StreamingCompareImageFilterType;
  typedef itk::ShiftScaleImageFilter <ImageType , ImageType > ShiftFilterType;

  // Instantiating object
  StreamingCompareImageFilterType::Pointer filter = StreamingCompareImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  ShiftFilterType::Pointer shiftFilter = ShiftFilterType::New();
  shiftFilter->SetInput (reader->GetOutput());
  shiftFilter->SetScale(1);
  shiftFilter->SetShift(10);

  filter->SetInput1(reader->GetOutput());
  filter->SetInput2(shiftFilter->GetOutput());
  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file << "MSE: " << filter->GetMSE() << std::endl;
  file << "MAE: " << filter->GetMAE() << std::endl;
  file << "PSNR: " << filter->GetPSNR() << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
