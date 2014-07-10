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
#include "otbInverseLogPolarTransform.h"
#include "otbImage.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include "itkPoint.h"
#include "otbMacro.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbInverseLogPolarTransformResample(int itkNotUsed(argc), char* argv[])
{
  char * inputFileName = argv[1];
  char * outputFileName = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                        PrecisionType;
  typedef unsigned char                                                 PixelType;
  typedef otb::Image<PixelType, Dimension>                              ImageType;
  typedef otb::InverseLogPolarTransform<PrecisionType>                  InverseLogPolarTransformType;
  typedef itk::LinearInterpolateImageFunction<ImageType, PrecisionType> InterpolatorType;
  typedef otb::ImageFileReader<ImageType>                               ReaderType;
  typedef otb::ImageFileWriter<ImageType>                               WriterType;
  typedef itk::ResampleImageFilter<ImageType, ImageType, PrecisionType> ResampleFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->UpdateOutputInformation();

  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  interpolator->SetInputImage(reader->GetOutput());

  std::cout << interpolator << std::endl;
  InverseLogPolarTransformType::Pointer transform = InverseLogPolarTransformType::New();

  ImageType::SizeType size;
  size[0] = 720;
  size[1] = 540;

  InverseLogPolarTransformType::ParametersType params(4);
  // Center the transform
  params[0] = 0.5 * static_cast<double>(size[0]);
  params[1] = 0.5 * static_cast<double>(size[1]);
  params[2] = 360. / reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  params[3] = vcl_log(vcl_sqrt(vcl_pow(static_cast<double>(size[0]), 2.)
                               + vcl_pow(static_cast<double>(size[1]),
                                         2.)) / 2) / reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
  transform->SetParameters(params);

  // ImageType::SpacingType spacing;
  //     spacing.Fill(1.0);

  ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  resampler->SetInput(reader->GetOutput());
  resampler->SetTransform(transform);
  resampler->SetInterpolator(interpolator);
  resampler->SetDefaultPixelValue(0);
  resampler->SetSize(size);

  ImageType::PointType origin;
  origin[0] = 0.5;
  origin[1] = 0.5;
  resampler->SetOutputOrigin(origin);

  ImageType::SpacingType spacing;
  spacing[0] = 1.0;
  spacing[1] = 1.0;
  resampler->SetOutputSpacing(spacing);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(resampler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
