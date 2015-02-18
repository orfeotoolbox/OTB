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

#include "otbRAndBAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbTSARVIRAndBAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> InputRImageType;
  typedef otb::Image<PixelType, Dimension> InputBImageType;
  typedef otb::Image<PixelType, Dimension> InputNIRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  typedef otb::ImageFileReader<InputRImageType>   RReaderType;
  typedef otb::ImageFileReader<InputBImageType>   BReaderType;
  typedef otb::ImageFileReader<InputNIRImageType> NIRReaderType;
  typedef otb::ImageFileWriter<OutputImageType>   WriterType;

  typedef otb::Functor::TSARVI<InputRImageType::PixelType,
      InputBImageType::PixelType,
      InputNIRImageType::PixelType,
      OutputImageType::PixelType> FunctorType;

  typedef otb::RAndBAndNIRIndexImageFilter<InputRImageType,
      InputBImageType,
      InputNIRImageType,
      OutputImageType,
      FunctorType> RAndBAndNIRIndexImageFilterType;

  // Instantiating object
  RAndBAndNIRIndexImageFilterType::Pointer filter = RAndBAndNIRIndexImageFilterType::New();
  RReaderType::Pointer                     readerR = RReaderType::New();
  BReaderType::Pointer                     readerB = BReaderType::New();
  NIRReaderType::Pointer                   readerNIR = NIRReaderType::New();
  WriterType::Pointer                      writer = WriterType::New();

  const char * inputFilenameR  = argv[1];
  const char * inputFilenameB  = argv[2];
  const char * inputFilenameNIR  = argv[3];
  const char * outputFilename = argv[4];

  double a(::atof(argv[5]));
  double b(::atof(argv[6]));
  double x(::atof(argv[7]));
  double gamma(::atof(argv[8]));

  readerR->SetFileName(inputFilenameR);
  readerB->SetFileName(inputFilenameB);
  readerNIR->SetFileName(inputFilenameNIR);
  writer->SetFileName(outputFilename);
  filter->SetInputR(readerR->GetOutput());
  filter->SetInputB(readerB->GetOutput());
  filter->SetInputNIR(readerNIR->GetOutput());

  filter->GetFunctor().SetA(a);
  filter->GetFunctor().SetB(b);
  filter->GetFunctor().SetX(x);
  filter->GetFunctor().SetGamma(gamma);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
