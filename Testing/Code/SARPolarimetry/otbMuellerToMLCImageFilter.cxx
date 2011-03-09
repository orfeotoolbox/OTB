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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMuellerToMLCImageFilter.h"
#include "otbSinclairImageFilter.h"
#include "otbSinclairToMuellerFunctor.h"
#include "otbComplexToVectorImageCastFilter.h"
#include "otbExtractROI.h"

int otbMuellerToMLCImageFilter(int argc, char * argv[])
{
  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * inputFilename3  = argv[3];
  const char * inputFilename4  = argv[4];

  const char * outputFilename = argv[5];

  typedef double                   PixelType;
  typedef std::complex<PixelType>  InputPixelType;

  const unsigned int Dimension = 2;

  
  typedef otb::Image<InputPixelType,  Dimension>       InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>       RealImageType;
  typedef otb::VectorImage<InputPixelType, Dimension>  ComplexImageType;
  typedef otb::Functor::SinclairToMuellerFunctor<
                      InputImageType::PixelType,
                      InputImageType::PixelType,
                      InputImageType::PixelType,
                      InputImageType::PixelType,
                      RealImageType::PixelType>       FunctionType;

  typedef otb::SinclairImageFilter<InputImageType, InputImageType,
                      InputImageType, InputImageType,
                      RealImageType, FunctionType >  SinclairToMuellerFilterType;
  typedef otb::MuellerToMLCImageFilter<RealImageType, ComplexImageType> FilterType;

  typedef otb::ComplexToVectorImageCastFilter<ComplexImageType, RealImageType> Castertype;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<RealImageType> WriterType;

  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  ReaderType::Pointer reader3 = ReaderType::New();
  ReaderType::Pointer reader4 = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  reader3->SetFileName(inputFilename3);
  reader4->SetFileName(inputFilename4);

  SinclairToMuellerFilterType::Pointer sinclairToMuellerFilter
                                       = SinclairToMuellerFilterType::New();
  sinclairToMuellerFilter->SetInputHH(reader1->GetOutput());
  sinclairToMuellerFilter->SetInputHV(reader2->GetOutput());
  sinclairToMuellerFilter->SetInputVH(reader3->GetOutput());
  sinclairToMuellerFilter->SetInputVV(reader4->GetOutput());

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(sinclairToMuellerFilter->GetOutput());

  Castertype::Pointer caster = Castertype::New();
  caster->SetInput(filter->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(caster->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
