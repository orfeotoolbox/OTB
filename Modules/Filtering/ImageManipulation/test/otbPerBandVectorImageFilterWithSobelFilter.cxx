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
#include "itkMacro.h"

#include "otbPerBandVectorImageFilter.h"
#include "otbVectorImage.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbPerBandVectorImageFilterWithSobelFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                   PixelType;
  typedef otb::Image<PixelType, Dimension>                         ImageType;
  typedef otb::VectorImage<PixelType, Dimension>                   VectorImageType;
  typedef itk::SobelEdgeDetectionImageFilter<ImageType, ImageType> SobelFilterType;

  typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, SobelFilterType>
  PerBandVectorImageFilterType;

  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<VectorImageType> WriterType;

  // Instantiating object
  PerBandVectorImageFilterType::Pointer filter = PerBandVectorImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(infname);
  writer->SetFileName(outfname);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
