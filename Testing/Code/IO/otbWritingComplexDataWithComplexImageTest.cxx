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
#include <complex>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "otbImage.h"
#include "otbMacro.h"

int otbWritingComplexDataWithComplexImageTest(int itkNotUsed(argc), char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::Image<PixelType, 2>              ImageType;
  typedef otb::ImageFileReader<ImageType>       ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;
  typedef otb::ExtractROI<PixelType, PixelType> ExtractROIFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  ExtractROIFilterType::Pointer extract = ExtractROIFilterType::New();
  extract->SetInput(reader->GetOutput());
  extract->SetStartX(atoi(argv[3]));
  extract->SetStartY(atoi(argv[4]));
  extract->SetSizeX(atoi(argv[5]));
  extract->SetSizeY(atoi(argv[6]));

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(extract->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
