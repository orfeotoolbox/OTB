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
#include "itkExceptionObject.h"

#include "otbRadarFunctors.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbTerraSarRadarBrightnessComplexImageFilterTest(int argc, char * argv[])
{

  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef std::complex<double>                                                                               ComplexPixelType;
  typedef otb::Image< ComplexPixelType >                                                                     ComplexImageType;

  typedef otb::Functor::TerraSarRadarBrightnessComplexImageFunctor< ComplexPixelType, ComplexPixelType >     FunctorType;
  typedef itk::UnaryFunctorImageFilter<ComplexImageType, ComplexImageType, FunctorType >                     FilterType;

  typedef otb::ImageFileReader<ComplexImageType>                                    ReaderType;
  typedef otb::ImageFileWriter<ComplexImageType>                                    WriterType;


  ReaderType::Pointer reader         = ReaderType::New();
  WriterType::Pointer writer         = WriterType::New();
  FilterType::Pointer filter         = FilterType::New();


  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();


  filter->GetFunctor().SetCalFactor( 10 );


  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
//   writer->SetNumberOfStreamDivisions(1);
  writer->Update();


  return EXIT_SUCCESS;
}
