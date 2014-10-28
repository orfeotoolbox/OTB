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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "otbLmvmPanSharpeningFusionImageFilter.h"

int otbLmvmPanSharpeningFusionImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * panchro = argv[1];
  const char * multispect = argv[2];
  const char * output = argv[3];

  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::VectorImage<PixelType, Dimension>         VectorImageType;
  typedef otb::Image<PixelType, Dimension>               PanchroImageType;
  typedef otb::ImageFileReader<VectorImageType>          VectorReaderType;
  typedef otb::ImageFileReader<PanchroImageType>         ImageReaderType;
  typedef otb::ImageFileWriter<VectorImageType> VectorImageWriterType;
  typedef otb::LmvmPanSharpeningFusionImageFilter
    <PanchroImageType, VectorImageType,  VectorImageType, double> FilterType;

  VectorReaderType::Pointer      multiSpectReader = VectorReaderType::New();
  ImageReaderType::Pointer       panchroReader = ImageReaderType::New();
  FilterType::Pointer            filter = FilterType::New();
  VectorImageWriterType::Pointer writer = VectorImageWriterType::New();

  multiSpectReader->SetFileName(multispect);
  panchroReader->SetFileName(panchro);

  PanchroImageType::SizeType radius;
  radius[0] = 5;
  radius[1] = 5;
  itk::Array<double> filterCoeffs;
  filterCoeffs.SetSize((2 * radius[0] + 1) * (2 * radius[1] + 1));

  //       double filterTmp[] = {
  //         0.00390625, 0.0078125, 0.0117188, 0.015625, 0.0117188, 0.0078125, 0.00390625,
  //         0.0078125, 0.015625, 0.0234375, 0.03125, 0.0234375, 0.015625, 0.0078125,
  //         0.0117188, 0.0234375, 0.0351562, 0.046875, 0.0351562, 0.0234375, 0.0117188,
  //         0.015625, 0.03125, 0.046875, 0.0625, 0.046875, 0.03125, 0.015625,
  //         0.0117188, 0.0234375, 0.0351562, 0.046875, 0.0351562, 0.0234375, 0.0117188,
  //         0.0078125, 0.015625, 0.0234375, 0.03125, 0.0234375, 0.015625, 0.0078125,
  //         0.00390625, 0.0078125, 0.0117188, 0.015625, 0.0117188, 0.0078125, 0.00390625  };
  //
  //       filterCoeffs.SetData(filterTmp);

  filterCoeffs.Fill(1);

  filter->SetXsInput(multiSpectReader->GetOutput());
  filter->SetPanInput(panchroReader->GetOutput());
  filter->SetRadius(radius);
  filter->SetFilter(filterCoeffs);
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(output);
  writer->Update();

  return EXIT_SUCCESS;
}
