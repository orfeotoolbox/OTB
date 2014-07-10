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
#include "otbImageOfVectorsToMonoChannelExtractROI.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbImageOfVectorsToMonoChannelExtractROI(int itkNotUsed(argc), char * argv[])
{
  const int Dimension = 2;
  typedef unsigned char                         ScalarPixelType;
  typedef itk::Vector<unsigned char, Dimension> VectorPixelType;

  const char * inputImageFileName = argv[1];
  const char * outputImageFileName = argv[7];
  typedef otb::Image<ScalarPixelType, Dimension>                                       ScalarImageType;
  typedef otb::Image<VectorPixelType, Dimension>                                       VectorImageType;
  typedef otb::ImageOfVectorsToMonoChannelExtractROI<VectorImageType, ScalarImageType> FilterType;

  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ScalarImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageFileName);
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageFileName);
  // Instantiating object
  FilterType::Pointer object = FilterType::New();

  object->SetStartX(atoi(argv[2]));
  object->SetStartY(atoi(argv[3]));
  object->SetSizeX(atoi(argv[4]));
  object->SetSizeY(atoi(argv[5]));
  object->SetChannel(atoi(argv[6]));

  object->SetInput(reader->GetOutput());
  writer->SetInput(object->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
