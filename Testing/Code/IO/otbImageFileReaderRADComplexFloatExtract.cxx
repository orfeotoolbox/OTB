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
#include <iostream>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "otbComplexToVectorImageCastFilter.h"


int otbImageFileReaderRADComplexFloatExtract(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef std::complex<float> InputPixelType;
  typedef std::complex<float> OutputPixelType;

  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputCplxImageType;
  typedef otb::Image<float, Dimension>             OutputScalarImageType;
  typedef otb::VectorImage<float, Dimension>       OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::ComplexToVectorImageCastFilter<OutputCplxImageType, OutputImageType>          CasterType;


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  CasterType::Pointer caster = CasterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::ExtractROI<InputPixelType,
      OutputPixelType>  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetInput(reader->GetOutput());

  extractROIFilter->SetStartX(10);
  extractROIFilter->SetStartY(10);
  extractROIFilter->SetSizeX(100);
  extractROIFilter->SetSizeY(100);

  caster->SetInput(extractROIFilter->GetOutput());

  writer->SetInput(caster->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
