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
#include <iostream>
#include <typeinfo>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSinclairImageFilter.h"
#include "otbSinclairToCovarianceMatrixFunctor.h"
#include "otbSinclairToCircularCovarianceMatrixFunctor.h"
#include "otbSinclairToCoherencyMatrixFunctor.h"
#include "otbSinclairToMuellerMatrixFunctor.h"
#include "otbComplexToVectorImageCastFilter.h"
#include "otbExtractROI.h"

int otbSinclairImageFilterNew(int argc, char * argv[])
{
  typedef std::complex<float>        ComplexType;
  typedef otb::Image<ComplexType, 2> CplxImageType;
  typedef otb::VectorImage<ComplexType, 2> VCplxImageType;

  typedef otb::SinclairImageFilter<CplxImageType, CplxImageType, CplxImageType, CplxImageType, VCplxImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();


  return EXIT_SUCCESS;
}


#define generic_SinclairImageFilterMacro(T_InputPixel, T_OutputPixel, T_Function, _argc, _argv) \
  const char * inputFilename1  = _argv[1]; \
  const char * inputFilename2  = _argv[2]; \
  const char * inputFilename3  = _argv[3]; \
  typedef T_InputPixel  InputPixelType; \
  typedef T_OutputPixel OutputPixelType; \
  typedef otb::Image<InputPixelType> InputImageType; \
  typedef otb::VectorImage<OutputPixelType> OutputImageType; \
  typedef otb::ImageFileReader<InputImageType> ReaderType; \
  typedef otb::ExtractROI<InputPixelType, InputPixelType > ExtractROIType; \
  typedef otb::SinclairImageFilter<InputImageType, InputImageType, InputImageType, InputImageType, OutputImageType, T_Function> FilterType; \
  typename FilterType::Pointer filter = FilterType::New(); \
  typename ReaderType::Pointer reader1 = ReaderType::New(); \
  typename ReaderType::Pointer reader2 = ReaderType::New(); \
  typename ReaderType::Pointer reader3 = ReaderType::New(); \
  typename ExtractROIType::Pointer  extract1 = ExtractROIType::New(); \
  typename ExtractROIType::Pointer  extract2 = ExtractROIType::New(); \
  typename ExtractROIType::Pointer  extract3 = ExtractROIType::New(); \
  extract1->SetStartX(10); \
  extract1->SetStartY(10); \
  extract1->SetSizeX(30); \
  extract1->SetSizeY(30); \
  extract2->SetStartX(10); \
  extract2->SetStartY(10); \
  extract2->SetSizeX(30); \
  extract2->SetSizeY(30); \
  extract3->SetStartX(10); \
  extract3->SetStartY(10); \
  extract3->SetSizeX(30); \
  extract3->SetSizeY(30); \
  reader1->SetFileName(inputFilename1); \
  reader2->SetFileName(inputFilename2); \
  reader3->SetFileName(inputFilename3); \
  extract1->SetInput(reader1->GetOutput()); \
  extract2->SetInput(reader3->GetOutput()); \
  extract3->SetInput(reader3->GetOutput()); \
  filter->SetInputHH(extract1->GetOutput()); \
  filter->SetInputHV(extract2->GetOutput()); \
  filter->SetInputVH(extract2->GetOutput()); \
  filter->SetInputVV(extract3->GetOutput()); \
  filter->UpdateOutputInformation();


template<class TInputPixel, class TOutputPixel, class TFunction>
int generic_SinclairImageFilter(int argc, char * argv[])
{
  const char * outputFilename = argv[4];

  typedef otb::VectorImage<TOutputPixel> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  generic_SinclairImageFilterMacro( TInputPixel, TOutputPixel, TFunction, argc, argv);
  
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);

  writer->SetInput( filter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}

template<class TInputPixel, class TOutputPixel, class TFunction>
int generic_SinclairImageFilterWithCast(int argc, char * argv[])
{
  const char * outputFilename = argv[4];

  typedef otb::VectorImage<TOutputPixel> OutputImageType;
  typedef otb::VectorImage<typename TOutputPixel::value_type> OutputRealImageType;
  typedef otb::ImageFileWriter<OutputRealImageType> WriterType;
  typedef typename otb::ComplexToVectorImageCastFilter<OutputImageType, OutputRealImageType>  CasterType;

  generic_SinclairImageFilterMacro( TInputPixel, TOutputPixel, TFunction, argc, argv);

  typename WriterType::Pointer writer = WriterType::New();
  typename CasterType::Pointer caster = CasterType::New();

  writer->SetFileName(outputFilename);
  
  caster->SetInput( filter->GetOutput() );
  writer->SetInput( caster->GetOutput() );
  writer->Update();
  
  return EXIT_SUCCESS;
}


int otbSinclairImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef std::complex <double> InputPixelType;
  typedef std::complex <double> OutputPixelType;
  typedef double                OutputRealPixelType;

  typedef otb::Image<InputPixelType,  Dimension>       InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;
  typedef otb::VectorImage<OutputRealPixelType, Dimension> OutputRealImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "SinclairToCovarianceMatrix")
    return (generic_SinclairImageFilterWithCast<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCovarianceMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCircularCovarianceMatrix")
    return (generic_SinclairImageFilterWithCast<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCircularCovarianceMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCoherencyMatrix")
    return (generic_SinclairImageFilterWithCast<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCoherencyMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToMuellerMatrix")
    return (generic_SinclairImageFilter<InputPixelType, OutputRealPixelType,
                otb::Functor::SinclairToMuellerMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputRealImageType::PixelType> >
                  (argc, argv));
  else return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
