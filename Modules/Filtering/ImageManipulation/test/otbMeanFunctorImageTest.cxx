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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

#include "itkUnaryFunctorImageFilter.h"
#include "otbMeanFunctor.h"

int otbMeanFunctorImageTest(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef unsigned char                          PixelType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::ImageFileReader<VectorImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType>        WriterType;

  typedef otb::Functor::MeanFunctor<VectorImageType::PixelType, ImageType::PixelType>
  MeanFunctorType;

  typedef itk::UnaryFunctorImageFilter<VectorImageType, ImageType, MeanFunctorType> MeanFilterType;
  MeanFilterType::Pointer filter = MeanFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
