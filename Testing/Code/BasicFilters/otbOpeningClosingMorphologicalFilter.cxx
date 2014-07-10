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

#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbOpeningClosingMorphologicalFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  const unsigned int Radius = atoi(argv[3]);

  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType, OutputImageType,
      StructuringElementType> OpeningClosingFilterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Creating Kernel
  StructuringElementType structElt;
  structElt.SetRadius(Radius);
  structElt.CreateStructuringElement();

  // Instantiating the opening closing filter
  OpeningClosingFilterType::Pointer openingClosing = OpeningClosingFilterType::New();
  openingClosing->SetInput(reader->GetOutput());
  openingClosing->SetKernel(structElt);

  // Write the result to file
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(openingClosing->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
