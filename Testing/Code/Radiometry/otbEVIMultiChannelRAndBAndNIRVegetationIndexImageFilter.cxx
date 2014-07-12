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

#include "otbMultiChannelRAndBAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVegetationIndicesFunctor.h"

template<class TInputImage, class TOutputImage, class TFunction>
int generic_EVIMultiChannelRAndBAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  typedef otb::ImageFileReader<TInputImage>  ReaderType;
  typedef otb::ImageFileWriter<TOutputImage> WriterType;

  typedef otb::MultiChannelRAndBAndNIRIndexImageFilter<TInputImage, TOutputImage, TFunction>
  MultiChannelRAndBAndNIRIndexImageFilterType;

  // Instantiating object
  typename MultiChannelRAndBAndNIRIndexImageFilterType::Pointer filter =
    MultiChannelRAndBAndNIRIndexImageFilterType::New();
  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  unsigned int redChannel(::atoi(argv[3]));
  unsigned int blueChannel(::atoi(argv[4]));
  unsigned int nirChannel(::atoi(argv[5]));

  double g(::atof(argv[6]));
  double c1(::atof(argv[7]));
  double c2(::atof(argv[8]));
  double l(::atof(argv[9]));

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  filter->SetRedIndex(redChannel);
  filter->SetBlueIndex(blueChannel);
  filter->SetNIRIndex(nirChannel);
  filter->SetInput(reader->GetOutput());
  filter->GetFunctor().SetG(g);
  filter->GetFunctor().SetC1(c1);
  filter->GetFunctor().SetC2(c2);
  filter->GetFunctor().SetL(l);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double, Dimension> InputImageType;
  typedef otb::Image<double, Dimension>       OutputImageType;

  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if (strArgv == "EVI")
    return (generic_EVIMultiChannelRAndBAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
                otb::Functor::EVI<InputImageType::
                    InternalPixelType,
                    InputImageType::
                    InternalPixelType,
                    InputImageType::
                    InternalPixelType,
                    OutputImageType::PixelType> >
              (argc, argv));
  else return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
