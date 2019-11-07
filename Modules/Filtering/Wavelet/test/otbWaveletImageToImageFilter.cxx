/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
This test code will run a WaveletImageFilter on given input image. Output
from this filter is then fed to WaveletInverseImageFilter which does
the inverse transform. Hence output of inverse transform passed to
ImageFileWriter will result in the input image.

Wavelet operator used is HAAR (otb::Wavelet::HAAR).
This filter works only in single-threaded setup
*/

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbWaveletImageFilter.h"
#include "otbWaveletInverseImageFilter.h"

int otbWaveletImageToImageFilter(int argc, char* argv[])
{

  const char* inputFileName  = argv[1];
  const char* outputFileName = argv[argc - 1];

  /* Wavelet operator */
  const otb::Wavelet::Wavelet OperatorType = otb::Wavelet::HAAR;

  const int      Dimension = 2;
  typedef double PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;


  typedef otb::WaveletImageFilter<ImageType, ImageType, OperatorType> FwdFilterType;

  typedef otb::WaveletInverseImageFilter<ImageType, ImageType, OperatorType> InvFilterType;

  typedef otb::ImageFileWriter<ImageType> WriterType;

  //  itk::MultiThreader::SetGlobalMaximumNumberOfThreads( 1 );

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  /* Forward Transformation */
  FwdFilterType::Pointer fwdFilter = FwdFilterType::New();

  fwdFilter->SetInput(reader->GetOutput());
  fwdFilter->Update();

  /* Inverse Transformation */

  InvFilterType::Pointer invFilter = InvFilterType::New();

  invFilter->SetInput(fwdFilter->GetOutput());
  invFilter->Update();

  /* Writing output */
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(invFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
