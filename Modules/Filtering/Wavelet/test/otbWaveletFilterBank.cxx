/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"

int otbWaveletFilterBank(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFileName = argv[1];
  const unsigned int decimFactor = atoi(argv[2]); // 1 for multiscale, 2 for multiresolution
  const char *       outputFileName = argv[3];

  const int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  /* Wavelet choice */
  //const otb::Wavelet::Wavelet wvltID = otb::Wavelet::HAAR;
  const otb::Wavelet::Wavelet wvltID = otb::Wavelet::SYMLET8;

  /* Forward Transformation */
  typedef otb::WaveletOperator<wvltID, otb::Wavelet::FORWARD, PixelType, Dimension> WaveletOperator;
  typedef otb::WaveletFilterBank<ImageType, ImageType, WaveletOperator, otb::Wavelet::FORWARD>
  FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetSubsampleImageFactor(decimFactor);

  /* Inverse Transformation */
  typedef otb::WaveletOperator<wvltID, otb::Wavelet::INVERSE, PixelType, Dimension> InvWaveletOperator;
  typedef otb::WaveletFilterBank<ImageType, ImageType, InvWaveletOperator, otb::Wavelet::INVERSE>
  InvFilterType;
  InvFilterType::Pointer invFilter = InvFilterType::New();
  for (unsigned int i = 0; i < filter->GetNumberOfOutputs(); ++i)
    invFilter->SetInput(i, filter->GetOutput(i));
  invFilter->SetSubsampleImageFactor(filter->GetSubsampleImageFactor());

  /* Saving output */
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(invFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
