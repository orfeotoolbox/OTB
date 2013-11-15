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
#include "otbLandsatTMSpectralRuleBasedClassifier.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include <algorithm>

int otbLandsatTMSpectralRuleBasedClassifierTest(int argc, char * argv[])
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

  typedef otb::Functor::LandsatTM::SpectralRuleBasedClassifier<InputImageType::PixelType, OutputPixelType> FunctorType;

  typedef itk::UnaryFunctorImageFilter< InputImageType, OutputImageType, FunctorType > FilterType;

  FilterType::Pointer filter = FilterType::New();
  (filter->GetFunctor()).SetDegree(otb::Functor::LandsatTM::HundredsKelvin);
  (filter->GetFunctor()).SetReflectance(otb::Functor::LandsatTM::Thousands);
  (filter->GetFunctor()).SetSAT(otb::Functor::LandsatTM::L5);

  filter->SetInput( reader->GetOutput() );

  writer->SetInput( filter->GetOutput() );

  writer->Update();


  return EXIT_SUCCESS;
}
