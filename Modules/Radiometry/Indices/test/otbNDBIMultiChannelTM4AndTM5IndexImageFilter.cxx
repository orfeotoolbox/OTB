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

#include "itkUnaryFunctorImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbBuiltUpIndicesFunctor.h"

int otbNDBIMultiChannelTM4AndTM5IndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double, Dimension>   InputImageType;
  typedef otb::Image<double, Dimension>         OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::Functor::NDBI <InputImageType::InternalPixelType,
      InputImageType::InternalPixelType,
      OutputImageType::PixelType> FunctorType;

  typedef itk::UnaryFunctorImageFilter<InputImageType, OutputImageType, FunctorType>
  UnaryFunctorImageFilterType;

  // Instantiating object
  UnaryFunctorImageFilterType::Pointer filter = UnaryFunctorImageFilterType::New();
  ReaderType::Pointer                  reader = ReaderType::New();
  WriterType::Pointer                  writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  unsigned int tm4Channel(::atoi(argv[3]));
  unsigned int tm5Channel(::atoi(argv[4]));

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  filter->GetFunctor().SetIndex1(tm4Channel);
  filter->GetFunctor().SetIndex2(tm5Channel);
  //filter->GetFunctor().SetTM5Index(tm5Channel);
  //filter->GetFunctor().SetTM4Index(tm4Channel);
  filter->SetInput(reader->GetOutput());

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
