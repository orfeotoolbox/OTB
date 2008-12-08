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
#include "otbRationalQuotientResampleImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbRationalQuotientResampleImageFilter(int argc, char* argv[])
{
  const char* inputName = argv[1];
  const char* outputName = argv[2];
  const unsigned int numerator = atoi(argv[3]);
  const unsigned int denominator = atoi(argv[4]);

  typedef float PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType,Dimension> ImageType;

  // Reader
  typedef otb::ImageFileReader<ImageType> ReaderType;
  // Writer
  typedef otb::ImageFileWriter<ImageType> WriterType;

  // Rational quotient resample
  typedef otb::RationalQuotientResampleImageFilter<ImageType,ImageType> ResampleType;

  // Instanciation object
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  ResampleType::Pointer resampler = ResampleType::New();

  // Pipe reader and resampler
  reader->SetFileName(inputName);
  resampler->SetInput(reader->GetOutput());

  // Set filter properties, eg rational quotient
  resampler->SetNumeratorFactors(numerator);
  resampler->SetDenominatorFactors(denominator);

  // Pipe resampler and writer
  writer->SetFileName(outputName);
  writer->SetInput(resampler->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
