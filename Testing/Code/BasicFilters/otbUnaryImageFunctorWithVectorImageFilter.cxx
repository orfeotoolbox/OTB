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

#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkCastImageFilter.h"

int otbUnaryImageFunctorWithVectorImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                   PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                   InputImageType;
  typedef InputImageType::InternalPixelType                        InternalPixelType;
  typedef itk::Functor::Cast<InternalPixelType, InternalPixelType> FunctorType;
  typedef otb::VectorImage<PixelType, Dimension>                   OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                     ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                    WriterType;
  typedef otb::UnaryImageFunctorWithVectorImageFilter<InputImageType,
      InputImageType,
      FunctorType>    UnaryImageFunctorWithVectorImageFilterType;

  // Instantiating object
  UnaryImageFunctorWithVectorImageFilterType::Pointer filter = UnaryImageFunctorWithVectorImageFilterType::New();
  ReaderType::Pointer                                 reader  = ReaderType::New();
  WriterType::Pointer                                 writer = WriterType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
