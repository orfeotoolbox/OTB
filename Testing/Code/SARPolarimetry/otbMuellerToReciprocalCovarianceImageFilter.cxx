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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMuellerToReciprocalCovarianceImageFilter.h"
#include "otbExtractROI.h"

int otbMuellerToReciprocalCovarianceImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double                   PixelType;
  typedef std::complex<PixelType>  ComplexPixelType;

  typedef otb::VectorImage<PixelType>                                          RealImageType;
  typedef otb::VectorImage<ComplexPixelType>                                   ComplexImageType;
  typedef otb::MuellerToReciprocalCovarianceImageFilter<RealImageType, ComplexImageType> FilterType;

  typedef otb::ImageFileReader<RealImageType>  ReaderType;
  typedef otb::ImageFileWriter<ComplexImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename );

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
