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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkTernaryFunctorImageFilter.h"
#include "otbAmplitudePhaseToRGBFunctor.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkComplexToPhaseImageFilter.h"
#include "itkShiftScaleImageFilter.h"

int otbAmplitudePhaseToRGBFunctor(int argc, char * argv[])
{
  typedef float                    PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef std::complex<PixelType>         ComplexPixelType;
  typedef otb::Image<ComplexPixelType, 2> ComplexImageType;

  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>  RGBImageType;

  typedef otb::ImageFileReader<ComplexImageType>      ReaderType;
  typedef otb::ImageFileWriter<RGBImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  typedef itk::ComplexToModulusImageFilter<ComplexImageType, ImageType> ModulusFilterType;
  ModulusFilterType::Pointer modulusFilter = ModulusFilterType::New();
  modulusFilter->SetInput(reader->GetOutput());

  typedef itk::ComplexToPhaseImageFilter<ComplexImageType, ImageType> PhaseFilterType;
  PhaseFilterType::Pointer phaseFilter = PhaseFilterType::New();
  phaseFilter->SetInput(reader->GetOutput());

  typedef itk::ShiftScaleImageFilter<ImageType, ImageType> ConstFilterType;
  ConstFilterType::Pointer constFilter = ConstFilterType::New();
  constFilter->SetScale(0.0);
  constFilter->SetInput(modulusFilter->GetOutput());

  typedef otb::Functor::AmplitudePhaseToRGBFunctor
  <PixelType, PixelType, PixelType, RGBPixelType> ColorMapFunctorType;
  typedef itk::TernaryFunctorImageFilter
  <ImageType, ImageType, ImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->GetFunctor().SetMaximum(4000);
  colormapper->GetFunctor().SetMinimum(0);

  colormapper->SetInput1(modulusFilter->GetOutput());
  colormapper->SetInput2(constFilter->GetOutput());
  colormapper->SetInput3(phaseFilter->GetOutput());
  colormapper->SetNumberOfThreads(1);

  writer->SetInput(colormapper->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
