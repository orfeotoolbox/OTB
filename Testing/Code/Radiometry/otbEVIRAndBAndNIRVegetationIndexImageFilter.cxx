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

#include "otbRAndBAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVegetationIndicesFunctor.h"

template<class TInputRImage, class TInputBImage, class TInputNIRImage, class TOutputImage, class TFunction>
int generic_EVIRAndBAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  typedef otb::ImageFileReader<TInputRImage>   RReaderType;
  typedef otb::ImageFileReader<TInputBImage>   BReaderType;
  typedef otb::ImageFileReader<TInputNIRImage> NIRReaderType;
  typedef otb::ImageFileWriter<TOutputImage>   WriterType;

  typedef otb::RAndBAndNIRIndexImageFilter<TInputRImage, TInputBImage, TInputNIRImage, TOutputImage, TFunction>
  RAndBAndNIRIndexImageFilterType;

  // Instantiating object
  typename RAndBAndNIRIndexImageFilterType::Pointer filter = RAndBAndNIRIndexImageFilterType::New();
  typename RReaderType::Pointer readerR = RReaderType::New();
  typename BReaderType::Pointer readerB = BReaderType::New();
  typename NIRReaderType::Pointer readerNIR = NIRReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  const char * inputFilenameR  = argv[1];
  const char * inputFilenameB  = argv[2];
  const char * inputFilenameNIR  = argv[3];
  const char * outputFilename = argv[4];
  double       g(::atof(argv[5]));
  double       c1(::atof(argv[6]));
  double       c2(::atof(argv[7]));
  double       l(::atof(argv[8]));

  readerR->SetFileName(inputFilenameR);
  readerB->SetFileName(inputFilenameB);
  readerNIR->SetFileName(inputFilenameNIR);
  writer->SetFileName(outputFilename);
  filter->SetInputR(readerR->GetOutput());
  filter->SetInputB(readerB->GetOutput());
  filter->SetInputNIR(readerNIR->GetOutput());

  filter->GetFunctor().SetG(g);
  filter->GetFunctor().SetC1(c1);
  filter->GetFunctor().SetC2(c2);
  filter->GetFunctor().SetL(l);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbEVIRAndBAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> InputRImageType;
  typedef otb::Image<PixelType, Dimension> InputBImageType;
  typedef otb::Image<PixelType, Dimension> InputNIRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "EVI")
    return (generic_EVIRAndBAndNIRVegetationIndexImageFilter<InputRImageType, InputBImageType, InputNIRImageType,
                OutputImageType,
                otb::Functor::EVI<InputRImageType::PixelType,
                    InputBImageType::PixelType,
                    InputNIRImageType::PixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
