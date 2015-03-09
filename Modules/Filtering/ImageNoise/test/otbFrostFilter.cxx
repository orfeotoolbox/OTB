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


#include "otbImageFileReader.h"
#include "otbFrostImageFilter.h"

#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbFrostFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  unsigned int RadiusX((unsigned int) ::atoi(argv[3]));
  unsigned int RadiusY((unsigned int) ::atoi(argv[4]));
  double       Deramp ((double) ::atof(argv[5]));

  typedef double InputPixelType;
  typedef double OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::FrostImageFilter<InputImageType, OutputImageType> FilterType;

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusY;

  FilterType::Pointer filterFrost = FilterType::New();

  filterFrost->SetRadius(Radius);
  filterFrost->SetDeramp(Deramp);

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  filterFrost->SetInput(reader->GetOutput());
  writer->SetInput(filterFrost->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
