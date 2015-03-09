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
#include "otbGeodesicMorphologyDecompositionImageFilter.h"
#include "otbGeodesicMorphologyDecompositionImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkBinaryBallStructuringElement.h"

int otbGeodesicMorphologyDecompositionImageFilter(int itkNotUsed(argc), char * argv[])
{

  const char *       infname = argv[1];
  const char *       convexfname = argv[2];
  const char *       concavefname = argv[3];
  const char *       levelingfname = argv[4];
  const unsigned int sesize = atoi(argv[5]);

  typedef otb::Image<double, 2> ImageType;
  typedef itk::BinaryBallStructuringElement<double,
      2>                                                  StructuringElementType;
  typedef StructuringElementType::RadiusType       RadiusType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::GeodesicMorphologyDecompositionImageFilter<ImageType, ImageType,
      StructuringElementType>
  GeodesicMorphologyDecompositionImageFilterType;

  // Instantiating object
  GeodesicMorphologyDecompositionImageFilterType::Pointer filter = GeodesicMorphologyDecompositionImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());

  RadiusType radius;
  radius.Fill(sesize);

  filter->SetRadius(radius);

  // writer the leveling map
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(levelingfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  // write the convex map
  writer = WriterType::New();
  writer->SetFileName(convexfname);
  writer->SetInput(filter->GetConvexMap());
  writer->Update();

  // write the concave map
  writer = WriterType::New();
  writer->SetFileName(concavefname);
  writer->SetInput(filter->GetConcaveMap());
  writer->Update();

  return EXIT_SUCCESS;
}
