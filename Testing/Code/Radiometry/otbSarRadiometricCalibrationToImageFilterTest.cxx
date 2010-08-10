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

#include "otbImage.h"
#include "otbSarRadiometricCalibrationToImageFilter.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

int otbSarRadiometricCalibrationToImageFilterTest(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                                         			   RealType;
  typedef std::complex<RealType>                                                       PixelType;
  typedef otb::Image<PixelType, Dimension>                                             InputImageType;
  typedef otb::Image<RealType, Dimension>                                              OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                         ReaderType;
  typedef otb::StreamingImageFileWriter<OutputImageType>                               WriterType;
  typedef otb::SarRadiometricCalibrationToImageFilter<InputImageType, OutputImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  filter->SetInput(reader->GetOutput());

  writer->SetInput(filter->GetOutput());
  writer->SetNumberOfStreamDivisions(2);
  writer->Update();

  return EXIT_SUCCESS;
}
