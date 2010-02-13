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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"

int otbWaveletTransform( int argc, char * argv[] )
{
  const char * inputFileName = argv[1];
  const unsigned int level = atoi(argv[2]);
  const unsigned int decimFactor = atoi(argv[3]);
  const char * outputFileName = argv[4];

  const int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::ImageFileReader< ImageType > ReaderType;

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );

  /* Wavelet choice */
  const otb::MotherWaveletOperatorEnum wvltID = otb::HAAR;

  /* Forward Transformation */
  typedef otb::WaveletOperator< wvltID, otb::Wavelet::FORWARD, PixelType, Dimension >
    WaveletOperator;
  typedef otb::WaveletFilterBank< ImageType, ImageType, WaveletOperator, otb::Wavelet::FORWARD >
    ForwardFilterBank;
  typedef otb::WaveletTransform< ImageType, ImageType, ForwardFilterBank, otb::Wavelet::FORWARD >
    FilterType;
  
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetNumberOfDecompositions( level );
  filter->SetSubsampleImageFactor( decimFactor );
  filter->Update();

  /* Inverse Transformation */
  typedef otb::WaveletOperator< wvltID, otb::Wavelet::INVERSE, PixelType, Dimension >
    InverseWaveletOperator;
  typedef otb::WaveletFilterBank< ImageType, ImageType, InverseWaveletOperator, otb::Wavelet::INVERSE >
    InverseFilterBank;
  typedef otb::WaveletTransform< ImageType, ImageType, InverseFilterBank, otb::Wavelet::INVERSE >
    InvFilterType;
  
  InvFilterType::Pointer invFilter = InvFilterType::New();
  invFilter->SetInput( filter->GetOutput() );
  invFilter->SetSubsampleImageFactor( decimFactor );
  invFilter->Update();

  /* Writing the output */
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputFileName );
  writer->SetInput( invFilter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}



