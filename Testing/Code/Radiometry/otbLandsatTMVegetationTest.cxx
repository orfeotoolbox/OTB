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
#include "itkFixedArray.h"
#include "otbLandsatTMIndices.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include <vector>
#include <algorithm>

int otbLandsatTMVegetationTest(int itkNotUsed(argc), char * argv[])
{

  typedef double InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::VectorImage< InputPixelType, 2 > InputImageType;
  typedef otb::Image< OutputPixelType, 2 > OutputImageType;

  typedef otb::ImageFileReader< InputImageType > ReaderType;
  typedef otb::ImageFileWriter< OutputImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );

  typedef otb::Functor::LandsatTM::VegetationSpectralRule<InputImageType::PixelType, OutputPixelType> R1FunctorType;
  R1FunctorType r1Funct = R1FunctorType();
  r1Funct.SetDegree(otb::Functor::LandsatTM::HundredsKelvin);
  r1Funct.SetReflectance(otb::Functor::LandsatTM::Thousands);

  typedef itk::UnaryFunctorImageFilter< InputImageType, OutputImageType, R1FunctorType > FilterType;

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( reader->GetOutput() );

  writer->SetInput( filter->GetOutput() );

  writer->Update();


  return EXIT_SUCCESS;
}
