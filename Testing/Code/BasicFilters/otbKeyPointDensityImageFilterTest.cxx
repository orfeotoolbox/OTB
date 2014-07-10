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

#include <stdio.h>

#include "otbKeyPointDensityImageFilter.h"
#include "otbImageToSIFTKeyPointSetFilter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbKeyPointDensityImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  const char *       infname = argv[1];
  const char *       outfname = argv[2];
  const unsigned int scales = atoi(argv[3]);
  const unsigned int octaves = atoi(argv[4]);
  const unsigned int radius = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef otb::Image<PixelType, Dimension>                           ImageType;
  typedef ImageType::IndexType                                       IndexType;
  typedef otb::ImageFileReader<ImageType>                            ReaderType;
  typedef otb::ImageFileWriter<ImageType>                            WriterType;
  typedef itk::VariableLengthVector<PixelType>                       RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>                   PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType> DetectorType;

  typedef otb::KeyPointDensityImageFilter<ImageType, ImageType, DetectorType> FilterType;

  /**Instantiation of an object*/
  FilterType::Pointer   filter =     FilterType::New();
  DetectorType::Pointer detector = DetectorType::New();
  ReaderType::Pointer   reader = ReaderType::New();

  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  filter->SetInput(reader->GetOutput());
  detector = filter->GetDetector();
  detector->SetOctavesNumber(octaves);
  detector->SetScalesNumber(scales);

  filter->SetDetector(detector);
  filter->SetNeighborhoodRadius(radius);

  /** Writing the densty Image*/
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
