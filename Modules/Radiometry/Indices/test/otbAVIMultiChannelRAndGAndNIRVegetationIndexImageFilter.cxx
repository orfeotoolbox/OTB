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

#include "otbMultiChannelRAndGAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double, Dimension>   InputImageType;
  typedef otb::Image<double, Dimension>         OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::Functor::AVI<InputImageType::InternalPixelType,
      InputImageType::InternalPixelType,
      InputImageType::InternalPixelType,
      OutputImageType::PixelType> FunctorType;
  typedef otb::MultiChannelRAndGAndNIRIndexImageFilter<InputImageType, OutputImageType, FunctorType>
  MultiChannelRAndGAndNIRIndexImageFilterType;

  // Instantiating object
  MultiChannelRAndGAndNIRIndexImageFilterType::Pointer filter = MultiChannelRAndGAndNIRIndexImageFilterType::New();
  ReaderType::Pointer                                  reader = ReaderType::New();
  WriterType::Pointer                                  writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  unsigned int redChannel(::atoi(argv[3]));
  unsigned int greenChannel(::atoi(argv[4]));
  unsigned int nirChannel(::atoi(argv[5]));

  double lg(::atof(argv[6]));
  double lr(::atof(argv[7]));
  double lnir(::atof(argv[8]));

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  filter->SetRedIndex(redChannel);
  filter->SetGreenIndex(greenChannel);
  filter->SetNIRIndex(nirChannel);
  filter->SetInput(reader->GetOutput());

  filter->GetFunctor().SetLambdaG(lg);
  filter->GetFunctor().SetLambdaR(lr);
  filter->GetFunctor().SetLambdaNir(lnir);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
