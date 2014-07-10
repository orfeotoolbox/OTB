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
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbBinaryImageDensityFunction.h"
#include "otbBinaryImageToDensityImageFilter.h"

int otbBinaryImageToDensityImageFilter(int itkNotUsed(argc), char* argv[])
{

  const char *       infname        = argv[1];
  const char *       outfname       = argv[2];
  const unsigned int radius         = atoi(argv[3]);

  const unsigned int Dimension = 2;
  typedef float                                PixelType;
  typedef otb::Image<PixelType, Dimension>     InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;

  typedef otb::BinaryImageDensityFunction<InputImageType>                                         CountFunctionType;
  typedef otb::BinaryImageToDensityImageFilter<InputImageType, InputImageType, CountFunctionType> FilterType;

  /** Instantiation of a Smart Pointer*/
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  /** */
  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->SetNeighborhoodRadius(radius);

  /** */
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
