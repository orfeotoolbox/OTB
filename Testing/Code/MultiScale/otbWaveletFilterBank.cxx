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

#include "otbWaveletFilterBank.h"

#include "otbHaarOperator.h"
#include "otbSplineBiOrthogonalOperator.h"

int otbWaveletFilterBank( int argc, char * argv[] )
{
  const char * inputFileName = argv[1];
  const char * prefix = argv[2];

  const int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::ImageFileReader< ImageType > ReaderType;

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );

  /* Transformation */
  typedef otb::LowPassHaarOperator< otb::FORWARD, PixelType, Dimension > LowPassOperator;
  typedef otb::HighPassHaarOperator< otb::FORWARD, PixelType, Dimension > HighPassOperator;
  //typedef otb::LowPassSplineBiOrthogonalOperator< otb::FORWARD, PixelType, Dimension > LowPassOperator;
  //typedef otb::HighPassSplineBiOrthogonalOperator< otb::FORWARD, PixelType, Dimension > HighPassOperator;

  typedef otb::WaveletFilterBank< ImageType, ImageType, LowPassOperator, HighPassOperator, otb::FORWARD >
    FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetSubsampleImageFactor(1);

  filter->Update();

  for ( unsigned int i = 0; i < filter->GetNumberOfOutputs(); i++ )
  {
    std::stringstream filename;
    filename << prefix;
    filename << "-" << i << ".tif";

    typedef otb::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(filename.str());
    writer->SetInput(filter->GetOutput(i));
    writer->Update();

  }
  return EXIT_SUCCESS;
}






