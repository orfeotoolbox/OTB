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

#include "otbRAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbWDVIRAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> InputRImageType;
  typedef otb::Image<PixelType, Dimension> InputNIRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  typedef otb::ImageFileReader<InputRImageType>   RReaderType;
  typedef otb::ImageFileReader<InputNIRImageType> NIRReaderType;
  typedef otb::ImageFileWriter<OutputImageType>   WriterType;

  typedef otb::Functor::WDVI<InputRImageType::PixelType,
      InputNIRImageType::PixelType,
      OutputImageType::PixelType> FunctorType;

  typedef otb::RAndNIRIndexImageFilter<InputRImageType,
      InputNIRImageType,
      OutputImageType,
      FunctorType> RAndNIRIndexImageFilterType;

  // Instantiating object
  RAndNIRIndexImageFilterType::Pointer filter = RAndNIRIndexImageFilterType::New();
  RReaderType::Pointer                 readerR = RReaderType::New();
  NIRReaderType::Pointer               readerNIR = NIRReaderType::New();
  WriterType::Pointer                  writer = WriterType::New();

  const char * inputFilenameR  = argv[1];
  const char * inputFilenameNIR  = argv[2];
  const char * outputFilename = argv[3];

  double s(::atof(argv[4]));

  readerR->SetFileName(inputFilenameR);
  readerNIR->SetFileName(inputFilenameNIR);
  writer->SetFileName(outputFilename);
  filter->SetInputR(readerR->GetOutput());
  filter->SetInputNIR(readerNIR->GetOutput());

  filter->GetFunctor().SetS(s);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
