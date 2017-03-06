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
#include "otbSarDeburstImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

typedef otb::Image<unsigned short>            ImageType;
typedef otb::ImageFileReader<ImageType>       ReaderType;
typedef otb::ImageFileWriter<ImageType>       WriterType;
typedef otb::SarDeburstImageFilter<ImageType> DeburstFilterType;

int otbSarDeburstFilterTest(int itkNotUsed(argc), char * argv[])
{
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  DeburstFilterType::Pointer filter = DeburstFilterType::New();
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();
  
  return EXIT_SUCCESS;
}

