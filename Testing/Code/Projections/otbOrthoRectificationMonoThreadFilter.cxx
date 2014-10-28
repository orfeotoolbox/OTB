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


// iostream is used for general output
#include <iostream>
#include <stdlib.h>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"

int otbOrthoRectificationMonoThreadFilter(int argc, char* argv[])
{
  if (argc != 12)
    {
    std::cout << argv[0] <<
    " <input filename> <output filename> <origin easting> <origin northing> <x size> <y size> <x spacing> <y spacing> <UTM zone <UTM hemisphere>"
              << std::endl;

    return EXIT_FAILURE;
    }

  typedef double                                   PixelType;
  typedef otb::Image<PixelType, 2>                 ImageType;
  typedef otb::VectorImage<PixelType, 2>           VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>    ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType>                   ExtractorType;
  typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;
  typedef otb::OrthoRectificationFilter<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterType;


  //Allocate pointer
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  ExtractorType::Pointer extractor = ExtractorType::New();

  OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
  UtmMapProjectionType::Pointer  utmMapProjection = UtmMapProjectionType::New();

  // Set parameters ...
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  reader->GenerateOutputInformation();

  // Extract the first channel of the image
  extractor->SetInput(reader->GetOutput());
  extractor->SetChannel(1);

  orthoRectifFilter->SetInput(extractor->GetOutput());

  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  orthoRectifFilter->SetOutputStartIndex(start);

  ImageType::SizeType size;
  size[0] = atoi(argv[5]);      // X size
  size[1] = atoi(argv[6]);            //Y size
  orthoRectifFilter->SetOutputSize(size);

  ImageType::SpacingType spacing;
  spacing[0] = atof(argv[7]);
  spacing[1] = atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);

  ImageType::PointType origin;
  origin[0] = strtod(argv[3], NULL);         //Origin easting
  origin[1] = strtod(argv[4], NULL);         //Origin northing
  orthoRectifFilter->SetOutputOrigin(origin);

  utmMapProjection->SetZone(atoi(argv[9]));
  utmMapProjection->SetHemisphere(argv[10][0]);
  orthoRectifFilter->SetMapProjection(utmMapProjection);

  // Displacement Field spacing
  ImageType::SpacingType  gridSpacing;
  gridSpacing[0] = atof(argv[11]);
  gridSpacing[1] = -atof(argv[11]);
  orthoRectifFilter->SetDisplacementFieldSpacing(gridSpacing);

  writer->SetInput(orthoRectifFilter->GetOutput());
  writer->SetAutomaticTiledStreaming();
  writer->SetNumberOfThreads(1);
  writer->Update();

  return EXIT_SUCCESS;
} //Fin main()
