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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMuellerToCovarianceImageFilter.h"
#include "otbComplexToVectorImageCastFilter.h"
#include "otbExtractROI.h"

int otbMuellerToCovarianceImageFilter(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double                   PixelType;
  typedef std::complex<PixelType>  ComplexPixelType;

  typedef otb::VectorImage<PixelType>                                          RealImageType;
  typedef otb::VectorImage<ComplexPixelType>                                   ComplexImageType;
  typedef otb::MuellerToCovarianceImageFilter<RealImageType, ComplexImageType> FilterType;
  typedef otb::ComplexToVectorImageCastFilter<ComplexImageType, RealImageType> Castertype;

  typedef otb::ImageFileReader<RealImageType>  ReaderType;
  typedef otb::ImageFileWriter<RealImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename );

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());

  Castertype::Pointer caster = Castertype::New();
  caster->SetInput(filter->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(caster->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
