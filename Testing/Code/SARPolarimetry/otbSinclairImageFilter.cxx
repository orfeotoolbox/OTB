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
#include "otbSinclairToReciprocalCovarianceFunctor.h"
#include "otbSinclairToCovarianceFunctor.h"
#include "otbSinclairToReciprocalCircularCovarianceMatrixFunctor.h"
#include "otbSinclairToCircularCovarianceMatrixFunctor.h"
#include "otbSinclairToReciprocalCoherencyFunctor.h"
#include "otbSinclairToCoherencyFunctor.h"
#include "otbSinclairToMuellerFunctor.h"
#include "otbComplexToVectorImageCastFilter.h"


#define generic_SinclairImageFilterMacro(T_InputPixel, T_OutputPixel, T_Function, _argc, _argv) \
  const char * inputFilename1  = _argv[1]; \
  const char * inputFilename2  = _argv[2]; \
  const char * inputFilename3  = _argv[3]; \
  typedef T_InputPixel  InputPixelType; \
  typedef T_OutputPixel OutputPixelType; \
  typedef otb::Image<InputPixelType> InputImageType; \
  typedef otb::VectorImage<OutputPixelType> OutputImageType; \
  typedef otb::ImageFileReader<InputImageType> ReaderType; \
  typedef otb::SinclairImageFilter<InputImageType, InputImageType, InputImageType, InputImageType, OutputImageType, T_Function> FilterType; \
  typename FilterType::Pointer filter = FilterType::New(); \
  typename ReaderType::Pointer reader1 = ReaderType::New(); \
  typename ReaderType::Pointer reader2 = ReaderType::New(); \
  typename ReaderType::Pointer reader3 = ReaderType::New(); \
  reader1->SetFileName(inputFilename1); \
  reader2->SetFileName(inputFilename2); \
  reader3->SetFileName(inputFilename3); \
  filter->SetInputHH(reader1->GetOutput()); \
  filter->SetInputHV(reader2->GetOutput()); \
  filter->SetInputVH(reader2->GetOutput()); \
  filter->SetInputVV(reader3->GetOutput()); \
  filter->UpdateOutputInformation(); \


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
  if (strArgv == "SinclairToReciprocalCovariance")
    return (generic_SinclairImageFilterWithCast<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToReciprocalCovarianceFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCovariance")
    return (generic_SinclairImageFilterWithCast<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCovarianceFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToReciprocalCircularCovarianceMatrix")
    return (generic_SinclairImageFilterWithCast<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToReciprocalCircularCovarianceMatrixFunctor<InputImageType::PixelType,
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
  else  if (strArgv == "SinclairToReciprocalCoherency")
    return (generic_SinclairImageFilterWithCast<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToReciprocalCoherencyFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCoherency")
    return (generic_SinclairImageFilterWithCast<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCoherencyFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToMueller")
    return (generic_SinclairImageFilter<InputPixelType, OutputRealPixelType,
                otb::Functor::SinclairToMuellerFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputRealImageType::PixelType> >
                  (argc, argv));
  else return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
