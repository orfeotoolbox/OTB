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
#include "otbEstimateInnerProductPCAImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbVectorImage.h"

int otbEstimateInnerProductPCAImageFilter(int itkNotUsed(argc), char* argv[])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  const char *       inputFileName = argv[1];
  const char *       outputFilename = argv[2];
  const unsigned int numberOfPrincipalComponentsRequired(atoi(argv[3]));
  const bool         centerdata = atoi(argv[4]);

  typedef otb::VectorImage<PixelType, Dimension>                        ImageType;
  typedef otb::ImageFileReader<ImageType>                               ReaderType;
  typedef otb::ImageFileWriter<ImageType>                               WriterType;
  typedef otb::EstimateInnerProductPCAImageFilter<ImageType, ImageType> PCAFilterType;

  ReaderType::Pointer reader     = ReaderType::New();
  reader->SetFileName(inputFileName);
  WriterType::Pointer writer     = WriterType::New();
  writer->SetFileName(outputFilename);

  PCAFilterType::Pointer pcafilter     = PCAFilterType::New();

  // Compute Inner Product raw
  pcafilter->SetNumberOfPrincipalComponentsRequired(numberOfPrincipalComponentsRequired);
  pcafilter->SetCenterData(centerdata);
  pcafilter->SetInput(reader->GetOutput());
  pcafilter->Update();

  writer->SetInput(pcafilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
