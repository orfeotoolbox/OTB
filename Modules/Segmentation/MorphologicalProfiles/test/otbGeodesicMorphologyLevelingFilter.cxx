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
#include "otbGeodesicMorphologyLevelingFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbGeodesicMorphologyLevelingFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname     = argv[1];
  const char * inconvfname = argv[2];
  const char * inconcfname = argv[3];
  const char * outfname    = argv[4];

  typedef otb::Image<double, 2>                                                  ImageType;
  typedef otb::GeodesicMorphologyLevelingFilter<ImageType, ImageType, ImageType> FilterType;
  typedef otb::ImageFileReader<ImageType>                                        ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                        WriterType;

  ReaderType::Pointer convreader, concreader, reader;
  reader = ReaderType::New();
  convreader = ReaderType::New();
  concreader = ReaderType::New();

  reader->SetFileName(infname);
  convreader->SetFileName(inconvfname);
  concreader->SetFileName(inconcfname);

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput(reader->GetOutput());
  filter->SetInputConvexMap(convreader->GetOutput());
  filter->SetInputConcaveMap(concreader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
