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

#include "otbMultiChannelRAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVegetationIndicesFunctor.h"

template<class TInputImage, class TOutputImage, class TFunction>
void generic_SetASetBMultiChannelRAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  typedef otb::ImageFileReader<TInputImage>  ReaderType;
  typedef otb::ImageFileWriter<TOutputImage> WriterType;

  typedef otb::MultiChannelRAndNIRIndexImageFilter<TInputImage, TOutputImage, TFunction>
  MultiChannelRAndNIRIndexImageFilterType;

  // Instantiating object
  typename MultiChannelRAndNIRIndexImageFilterType::Pointer filter = MultiChannelRAndNIRIndexImageFilterType::New();
  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();
  double       a(::atof(argv[1]));
  double       b(::atof(argv[2]));
  const char * inputFilename  = argv[3];
  const char * outputFilename = argv[4];
  unsigned int redChannel(::atoi(argv[5]));
  unsigned int nirChannel(::atoi(argv[6]));

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  filter->SetInput(reader->GetOutput());
  filter->SetRedIndex(redChannel);
  filter->SetNIRIndex(nirChannel);
  filter->GetFunctor().SetA(a);
  filter->GetFunctor().SetB(b);
  writer->SetInput(filter->GetOutput());
  writer->Update();
}

int otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<unsigned char, Dimension> InputImageType;
  typedef otb::Image<double, Dimension>              OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "PVI")
    generic_SetASetBMultiChannelRAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
        otb::Functor::PVI<InputImageType::InternalPixelType,
            InputImageType::InternalPixelType,
            OutputImageType::PixelType> >
      (argc, argv);

  return EXIT_SUCCESS;
}
