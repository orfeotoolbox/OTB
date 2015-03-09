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
#include <iterator>
#include <stdlib.h>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbStreamingResampleImageFilter.h"


#include "itkExtractImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkChangeInformationImageFilter.h"

#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"

int otbOrthoRectificationFilterWithDEM(int argc, char* argv[])
{
  if (argc != 13)
    {
    std::cout << argv[0] <<
    " <input filename> <output filename>  <origin easting>  <origin northing> <x size> <y size>";
    std::cout << "<x_spacing> <y_spacing> <UTM zone> <UTM hemisphere> <DEM directory>" << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::Image<double, 2>                    ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::UtmInverseProjection                                                 UtmMapProjectionType;
  typedef otb::OrthoRectificationFilter<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterType;

  //Allocate pointer
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  OrthoRectifFilterType::Pointer orthoRectifFilter = OrthoRectifFilterType::New();
  UtmMapProjectionType::Pointer  utmMapProjection = UtmMapProjectionType::New();

  // Set parameters ...
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  orthoRectifFilter->SetInput(reader->GetOutput());

  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  orthoRectifFilter->SetOutputStartIndex(start);

  ImageType::SizeType size;
  size[0] = atoi(argv[5]);      //X size.
  size[1] = atoi(argv[6]);      //Y size
  orthoRectifFilter->SetOutputSize(size);

  ImageType::SpacingType spacing;
  spacing[0] = atof(argv[7]);
  spacing[1] = atof(argv[8]);
  orthoRectifFilter->SetOutputSpacing(spacing);

  ImageType::PointType origin;
  origin[0] = strtod(argv[3], NULL);         //origin easting
  origin[1] = strtod(argv[4], NULL);         //origin northing
  orthoRectifFilter->SetOutputOrigin(origin);

  utmMapProjection->SetZone(atoi(argv[9]));
  utmMapProjection->SetHemisphere(argv[10][0]);
  orthoRectifFilter->SetMapProjection(utmMapProjection);

  // Displacement Field spacing
  ImageType::SpacingType  gridSpacing;
  gridSpacing[0] = atof(argv[12]);
  gridSpacing[1] = -atof(argv[12]);
  orthoRectifFilter->SetDisplacementFieldSpacing(gridSpacing);

  otb::DEMHandler::Instance()->OpenDEMDirectory(argv[11]);

  writer->SetInput(orthoRectifFilter->GetOutput());

  writer->SetAutomaticTiledStreaming();

  otbGenericMsgDebugMacro(<< "Update writer ...");
  writer->Update();

  return EXIT_SUCCESS;

} //End main()
