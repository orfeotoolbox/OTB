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

#include "otbGAndRIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSoilIndicesFunctor.h"

template<class TInputGImage, class TInputRImage, class TOutputImage, class TFunction>
int generic_GAndRIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  typedef otb::ImageFileReader<TInputGImage> GReaderType;
  typedef otb::ImageFileReader<TInputRImage> RReaderType;
  typedef otb::ImageFileWriter<TOutputImage> WriterType;

  typedef otb::GAndRIndexImageFilter<TInputGImage, TInputRImage, TOutputImage, TFunction>
  GAndRIndexImageFilterType;

  // Instantiating object
  typename GAndRIndexImageFilterType::Pointer filter = GAndRIndexImageFilterType::New();
  typename GReaderType::Pointer readerG = GReaderType::New();
  typename RReaderType::Pointer readerR = RReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  const char * inputFilenameG  = argv[1];
  const char * inputFilenameR  = argv[2];
  const char * outputFilename = argv[3];

  readerG->SetFileName(inputFilenameG);
  readerR->SetFileName(inputFilenameR);
  writer->SetFileName(outputFilename);
  filter->SetInputG(readerG->GetOutput());
  filter->SetInputR(readerR->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbGAndRIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                    PixelType;
  typedef otb::Image<PixelType, Dimension> InputGImageType;
  typedef otb::Image<PixelType, Dimension> InputRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "IR")
    return (generic_GAndRIndexImageFilter<InputGImageType, InputRImageType, OutputImageType,
                otb::Functor::IR<InputRImageType::PixelType,
                    InputRImageType::PixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else if (strArgv == "IC")
    return (generic_GAndRIndexImageFilter<InputGImageType, InputRImageType, OutputImageType,
                otb::Functor::IC<InputGImageType::PixelType,
                    InputRImageType::PixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else if (strArgv == "IB")
    return (generic_GAndRIndexImageFilter<InputGImageType, InputRImageType, OutputImageType,
                otb::Functor::IB<InputGImageType::PixelType,
                    InputRImageType::PixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
