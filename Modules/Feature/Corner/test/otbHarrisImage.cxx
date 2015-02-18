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
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbHarrisImageFilter.h"

int otbHarrisImage(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  double SigmaD((double) ::atof(argv[3]));
  double SigmaI((double) ::atof(argv[4]));
  double Alpha((double) ::atof(argv[5]));

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension>                  OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                    ReaderType;
  typedef otb::HarrisImageFilter<InputImageType, OutputImageType> FunctionType;
  typedef otb::ImageFileWriter<OutputImageType>                   WriterType;

  ReaderType::Pointer   reader   = ReaderType::New();
  WriterType::Pointer   writer   = WriterType::New();
  FunctionType::Pointer harris = FunctionType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  harris->SetInput(reader->GetOutput());
  harris->SetSigmaD(SigmaD);
  harris->SetSigmaI(SigmaI);
  harris->SetAlpha(Alpha);

  writer->SetInput(harris->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
