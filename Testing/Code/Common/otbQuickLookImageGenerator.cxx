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



#include "itkExceptionObject.h"
#include "otbQuickLookImageGenerator.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbQuickLookImageGenerator(int argc, char* argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef otb::VectorImage<double, 2>                        ImageType;
  typedef otb::ImageFileReader<ImageType>                    ReaderType;
  typedef otb::ImageFileWriter<ImageType>                    WriterType;
  typedef otb::QuickLookImageGenerator<ImageType, ImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFileName);

  filter->SetInput(reader->GetOutput());
  filter->SetSampleRatio(atoi(argv[3]));
  filter->SetVariance(atof(argv[4]));
  filter->SetMaximumError(atof(argv[5]));
  filter->SetMaximumKernelWidth(atoi(argv[6]));
  filter->UseImageSpacing(atoi(argv[7]));

  filter->Update();

  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outputFileName);

  writer->Update();

  return EXIT_SUCCESS;
}
