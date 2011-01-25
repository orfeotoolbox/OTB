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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSinclairImageFilter.h"
#include "otbSinclairToReciprocalCovarianceFunctor.h"
#include "otbSinclairToCoherencyFunctor.h"
#include "otbSinclairToMuellerFunctor.h"


template<class TInputPixel, class TOutputPixel, class TFunction>
int generic_SinclairImageFilter(int argc, char * argv[])
{
  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * inputFilename3  = argv[3];

  const char * outputFilename = argv[4];


  typedef TInputPixel  InputPixelType;
  typedef TOutputPixel OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>       InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::SinclairImageFilter<InputImageType, InputImageType,
                                      InputImageType, InputImageType,
                                      OutputImageType, TFunction > FilterType;

  typename FilterType::Pointer filter = FilterType::New();

  typename ReaderType::Pointer reader1 = ReaderType::New();
  typename ReaderType::Pointer reader2 = ReaderType::New();
  typename ReaderType::Pointer reader3 = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  reader3->SetFileName(inputFilename3);
  writer->SetFileName(outputFilename);

  filter->SetInputHH(reader1->GetOutput());
  filter->SetInputHV(reader2->GetOutput());
  filter->SetInputVH(reader2->GetOutput());
  filter->SetInputVV(reader3->GetOutput());
  writer->SetInput(filter->GetOutput());

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
    return (generic_SinclairImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToReciprocalCovarianceFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCoherency")
    return (generic_SinclairImageFilter<InputPixelType, OutputPixelType,
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
