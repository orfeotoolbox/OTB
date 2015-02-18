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
#include "otbNormalizeInnerProductPCAImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"

int otbNormalizeInnerProductPCAImageFilter(int itkNotUsed(argc), char* argv[])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  const char *       inputFileName = argv[1];
  const char *       outputFilename = argv[2];
  typedef otb::VectorImage<PixelType, Dimension>                         ImageType;
  typedef otb::ImageFileReader<ImageType>                                ReaderType;
  typedef otb::ImageFileWriter<ImageType>                       WriterType;
  typedef otb::NormalizeInnerProductPCAImageFilter<ImageType, ImageType> NormalizePCAFilterType;

  ReaderType::Pointer reader     = ReaderType::New();
  reader->SetFileName(inputFileName);
  WriterType::Pointer writer     = WriterType::New();
  writer->SetFileName(outputFilename);

  NormalizePCAFilterType::Pointer normalizepcafilter     = NormalizePCAFilterType::New();

  normalizepcafilter->SetInput(reader->GetOutput());
  writer->SetInput(normalizepcafilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
