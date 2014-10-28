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

#include "otbSarRadiometricCalibrationToImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbComplexToVectorImageCastFilter.h"

int otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                                        RealType;
  typedef std::complex<RealType>                                                       PixelType;
  typedef otb::Image<PixelType, Dimension>                                             InputImageType;
  typedef otb::Image<RealType, Dimension>                                              OutputImageType;

  typedef otb::ImageFileReader<InputImageType>                                         ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                                        WriterType;

  typedef otb::SarRadiometricCalibrationToImageFilter<InputImageType, OutputImageType> CalibFilterType;
  typedef otb::ExtractROI<RealType, RealType>                                          ExtractorType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  ExtractorType::Pointer extractor = ExtractorType::New();
  CalibFilterType::Pointer calibFilter = CalibFilterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  calibFilter->SetInput(reader->GetOutput());
  calibFilter->SetEnableNoise(false);

  if (argc > 3)
    {
    // Generate an extract from the large input
    OutputImageType::RegionType region;
    OutputImageType::IndexType  id;
    id[0] = atoi(argv[3]);   id[1] = atoi(argv[4]);
    OutputImageType::SizeType size;
    size[0] = atoi(argv[5]);   size[1] = atoi(argv[6]);
    region.SetIndex(id);
    region.SetSize(size);

    extractor->SetExtractionRegion(region);
    extractor->SetInput(calibFilter->GetOutput());
    writer->SetInput(extractor->GetOutput());
    }
  else
    {
    // Calibrate the whole image
    writer->SetInput(calibFilter->GetOutput());
    }
  writer->Update();

  return EXIT_SUCCESS;
}
