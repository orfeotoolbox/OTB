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

#include "otbSqrtSpectralAngleFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbSqrtSpectralAngleImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double, Dimension> InputImageType;
  typedef otb::Image<double, Dimension>       OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::Functor::SqrtSpectralAngleFunctor<InputImageType::PixelType, OutputImageType::PixelType> FunctorType;

  typedef itk::UnaryFunctorImageFilter<InputImageType, OutputImageType, FunctorType> SqrtSpectralAngleImageFilterType;

  // Instantiating objects
  SqrtSpectralAngleImageFilterType::Pointer filter = SqrtSpectralAngleImageFilterType::New();
  ReaderType::Pointer                       reader = ReaderType::New();
  WriterType::Pointer                       writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  unsigned int blueValue(::atoi(argv[3]));
  unsigned int greenValue(::atoi(argv[4]));
  unsigned int redValue(::atoi(argv[5]));
  unsigned int nirValue(::atoi(argv[6]));

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  InputImageType::PixelType referencePixel;
  referencePixel.SetSize(4);
  referencePixel[0] = blueValue;
  referencePixel[1] = greenValue;
  referencePixel[2] = redValue;
  referencePixel[3] = nirValue;

  filter->GetFunctor().SetReferencePixel(referencePixel);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;

}
