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

#include "itkBinaryFunctorImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbWaterIndicesFunctor.h"

int otbNDWIWaterIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> InputNIRImageType;
  typedef otb::Image<PixelType, Dimension> InputMIRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  typedef otb::ImageFileReader<InputNIRImageType> NIRReaderType;
  typedef otb::ImageFileReader<InputMIRImageType> MIRReaderType;
  typedef otb::ImageFileWriter<OutputImageType>   WriterType;

  typedef otb::Functor::NDWI  <InputNIRImageType::PixelType,
      InputMIRImageType::PixelType,
      OutputImageType::PixelType> FunctorType;

  typedef itk::BinaryFunctorImageFilter<InputNIRImageType,
      InputMIRImageType,
      OutputImageType,
      FunctorType> BinaryFunctorImageFilterType;

  // Instantiating object
  BinaryFunctorImageFilterType::Pointer filter = BinaryFunctorImageFilterType::New();
  NIRReaderType::Pointer                readerNIR = NIRReaderType::New();
  MIRReaderType::Pointer                readerMIR = MIRReaderType::New();
  WriterType::Pointer                   writer = WriterType::New();

  const char * inputFilenameNIR  = argv[1];
  const char * inputFilenameMIR  = argv[2];
  const char * outputFilename = argv[3];

  readerNIR->SetFileName(inputFilenameNIR);
  readerMIR->SetFileName(inputFilenameMIR);
  writer->SetFileName(outputFilename);
  filter->SetInput1(readerNIR->GetOutput());
  filter->SetInput2(readerMIR->GetOutput());

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
