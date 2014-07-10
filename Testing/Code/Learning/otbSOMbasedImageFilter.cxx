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
#include "otbSOMbasedImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbSOMMap.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkEuclideanDistanceMetric.h"


int otbSOMbasedImageFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * somfname = argv[2];
  const char * outfname = argv[3];

  const unsigned int Dimension = 2;

  typedef double                                          ComponentType;
  typedef itk::VariableLengthVector<ComponentType>         PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType>   DistanceType;
  typedef otb::VectorImage<ComponentType, Dimension>      ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::SOMMap<ImageType::PixelType>                                            SOMMapType;
  typedef otb::SOMbasedImageFilter<ImageType, ImageType, DistanceType, SOMMapType>     SOMbasedImageFilterType;
  typedef otb::ImageFileReader<ImageType>                                              ReaderType;
  typedef otb::ImageFileReader<SOMMapType>                                             SOMReaderType;
  typedef otb::ImageFileWriter<ImageType>                                     WriterType;

  // Instantiating object
  SOMbasedImageFilterType::Pointer filter = SOMbasedImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  SOMReaderType::Pointer somreader = SOMReaderType::New();
  somreader->SetFileName(somfname);
  somreader->Update();

  filter->SetMap(somreader->GetOutput());
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
