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

#include "otbStreamingResampleImageFilter.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkTranslationTransform.h"
#include "otbComplexToIntensityImageFilter.h"
#include "otbStreamingCompareImageFilter.h"

int otbResampleSLCImage(int itkNotUsed(argc), char * argv[])
{
  const char* inputFilename = argv[1];

  const unsigned int Dimension = 2;
  typedef std::complex<double> ComplexPixelType;
  typedef double               RealPixelType;

  typedef otb::Image<ComplexPixelType, Dimension>        ComplexImageType;
  typedef otb::Image<RealPixelType, Dimension>           RealImageType;
  typedef otb::ImageFileReader<ComplexImageType>         ReaderType;
  typedef itk::TranslationTransform<double, Dimension>   TransformType;
  typedef otb::ComplexToIntensityImageFilter<ComplexImageType, RealImageType> ComplexToIntensityFilterType;
  typedef otb::StreamingResampleImageFilter<ComplexImageType, ComplexImageType> ComplexResampleFilterType;
  typedef otb::StreamingResampleImageFilter<RealImageType, RealImageType> RealResampleFilterType;

  // Instantiating object
  ReaderType::Pointer         reader = ReaderType::New();
  ComplexToIntensityFilterType::Pointer beforeResampleConversion = ComplexToIntensityFilterType::New();
  ComplexToIntensityFilterType::Pointer afterResampleConversion = ComplexToIntensityFilterType::New();

  ComplexResampleFilterType::Pointer complexResample = ComplexResampleFilterType::New();
  RealResampleFilterType::Pointer realResample = RealResampleFilterType::New();
  TransformType::Pointer      transform = TransformType::New();

  typedef otb::StreamingCompareImageFilter<RealImageType> StreamingCompareImageFilterType;

  // Input Image
  reader->SetFileName(inputFilename);

  // Size of output resampler result
  ComplexResampleFilterType::SizeType size;
  size[0] = 200;
  size[1] = 200;

  // Transformation creation
  TransformType::OutputVectorType translation;
  translation[0] = 10;
  translation[1] = 20;
  transform->SetOffset(translation);

  // Image -> Resample -> Intensity image
  complexResample->SetInput(reader->GetOutput());
  complexResample->SetOutputSize(size);
  complexResample->SetTransform(transform);
  afterResampleConversion->SetInput(complexResample->GetOutput());

  // Image -> Intensity image -> Resample Image
  beforeResampleConversion->SetInput(reader->GetOutput());
  realResample->SetInput(beforeResampleConversion->GetOutput());
  realResample->SetTransform(transform);
  realResample->SetOutputSize(size);

  // Compare results
  StreamingCompareImageFilterType::Pointer compareImage = StreamingCompareImageFilterType::New();
  compareImage->SetInput1(afterResampleConversion->GetOutput());
  compareImage->SetInput2(realResample->GetOutput());
  compareImage->Update();

  if( compareImage->GetMAE() )
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
