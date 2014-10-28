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


#include <iostream>
#include <typeinfo>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSinclairImageFilter.h"
#include "otbSinclairToCircularCovarianceMatrixFunctor.h"
#include "otbSinclairToCoherencyMatrixFunctor.h"
#include "otbSinclairToMuellerMatrixFunctor.h"
#include "otbMultiChannelExtractROI.h"


int otbSinclairImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef std::complex<float>        ComplexType;
  typedef otb::Image<ComplexType, 2> CplxImageType;
  typedef otb::VectorImage<ComplexType, 2> VCplxImageType;

  typedef otb::SinclairImageFilter<CplxImageType, CplxImageType, CplxImageType, CplxImageType, VCplxImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();


  return EXIT_SUCCESS;
}


template<class TInputPixel, class TOutputPixel, class TFunction>
int generic_SinclairImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * outputFilename = argv[4];

  typedef otb::VectorImage<TOutputPixel> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * inputFilename3  = argv[3];

  typedef otb::Image<TInputPixel> InputImageType;
  typedef otb::VectorImage<TOutputPixel> OutputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::MultiChannelExtractROI<TOutputPixel, TOutputPixel > ExtractROIType;
  typedef otb::SinclairImageFilter<InputImageType, InputImageType, InputImageType, InputImageType, OutputImageType, TFunction> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  typename ReaderType::Pointer reader1 = ReaderType::New();
  typename ReaderType::Pointer reader2 = ReaderType::New();
  typename ReaderType::Pointer reader3 = ReaderType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  reader3->SetFileName(inputFilename3);
  filter->SetInputHH(reader1->GetOutput());
  filter->SetInputHV(reader2->GetOutput());
  filter->SetInputVH(reader2->GetOutput());
  filter->SetInputVV(reader3->GetOutput());

  filter->UpdateOutputInformation();

  typename ExtractROIType::Pointer  extract = ExtractROIType::New();
  extract->SetStartX(10);
  extract->SetStartY(10);
  extract->SetSizeX(30);
  extract->SetSizeY(30);
  extract->SetInput(filter->GetOutput());

  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);

  writer->SetInput( extract->GetOutput() );
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
    return (generic_SinclairImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCovarianceMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCircularCovarianceMatrix")
    return (generic_SinclairImageFilter<InputPixelType, OutputPixelType,
                otb::Functor::SinclairToCircularCovarianceMatrixFunctor<InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    InputImageType::PixelType,
                                    OutputImageType::PixelType> >
                  (argc, argv));
  else  if (strArgv == "SinclairToCoherencyMatrix")
    return (generic_SinclairImageFilter<InputPixelType, OutputPixelType,
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
