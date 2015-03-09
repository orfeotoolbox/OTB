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

#include "otbRAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

template<class TInputRImage, class TInputNIRImage, class TOutputImage, class TFunction>
void generic_SetASetBRAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  typedef otb::ImageFileReader<TInputRImage>   RReaderType;
  typedef otb::ImageFileReader<TInputNIRImage> NIRReaderType;
  typedef otb::ImageFileWriter<TOutputImage>   WriterType;

  typedef otb::RAndNIRIndexImageFilter<TInputRImage, TInputNIRImage, TOutputImage, TFunction>
  RAndNIRIndexImageFilterType;

  // Instantiating object
  typename RAndNIRIndexImageFilterType::Pointer filter = RAndNIRIndexImageFilterType::New();
  typename RReaderType::Pointer readerR = RReaderType::New();
  typename NIRReaderType::Pointer readerNIR = NIRReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();
  double       a(::atof(argv[1]));
  double       b(::atof(argv[2]));
  const char * inputFilenameR  = argv[3];
  const char * inputFilenameNIR  = argv[4];
  const char * outputFilename = argv[5];

  readerR->SetFileName(inputFilenameR);
  readerNIR->SetFileName(inputFilenameNIR);
  writer->SetFileName(outputFilename);
  filter->SetInputR(readerR->GetOutput());
  filter->SetInputNIR(readerNIR->GetOutput());
  filter->GetFunctor().SetA(a);
  filter->GetFunctor().SetB(b);
  writer->SetInput(filter->GetOutput());
  writer->Update();
}

int otbSetASetBRAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                    PixelType;
  typedef otb::Image<PixelType, Dimension> InputRImageType;
  typedef otb::Image<PixelType, Dimension> InputNIRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "PVI")
    generic_SetASetBRAndNIRVegetationIndexImageFilter<InputRImageType, InputNIRImageType, OutputImageType,
        otb::Functor::PVI<InputRImageType::PixelType,
            InputNIRImageType::PixelType,
            OutputImageType::PixelType> >
      (argc, argv);
  return EXIT_SUCCESS;
}
