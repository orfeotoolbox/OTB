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
#include "otbVectorImage.h"
#include "otbImage.h"

#include "otbBayesianFusionFilter.h"

int otbBayesianFusionFilter(int argc, char * argv[])
{
  const char * multispect = argv[1];
  const char * multispectinterp = argv[2];
  const char * panchro = argv[3];
  const char * output = argv[4];

  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::VectorImage<PixelType,
      Dimension>
  VectorImageType;
  typedef otb::Image<PixelType,
      Dimension>
  PanchroImageType;
  typedef otb::ImageFileReader<VectorImageType>
  VectorReaderType;
  typedef otb::ImageFileReader<PanchroImageType>
  ImageReaderType;
  typedef otb::ImageFileWriter<VectorImageType>
  VectorImageWriterType;
  typedef otb::BayesianFusionFilter<VectorImageType, VectorImageType, PanchroImageType, VectorImageType> FilterType;

  VectorReaderType::Pointer      multiSpectReader = VectorReaderType::New();
  VectorReaderType::Pointer      multiSpectInterpReader = VectorReaderType::New();
  ImageReaderType::Pointer       panchroReader = ImageReaderType::New();
  FilterType::Pointer            filter = FilterType::New();
  VectorImageWriterType::Pointer writer = VectorImageWriterType::New();

  multiSpectReader->SetFileName(multispect);
  multiSpectInterpReader->SetFileName(multispectinterp);
  panchroReader->SetFileName(panchro);

  filter->SetMultiSpect(multiSpectReader->GetOutput());
  filter->SetMultiSpectInterp(multiSpectInterpReader->GetOutput());
  filter->SetPanchro(panchroReader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(output);
  writer->Update();

  return EXIT_SUCCESS;
}
