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

#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbTextureFunctors.h"

template<class TInputImage, class TOutputImage, class TFunctor>
int generic_TextureFunctor(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef typename TInputImage::SizeType              SizeType;
  typedef typename TInputImage::OffsetType            OffsetType;
  typedef otb::ImageFileReader<TInputImage>           ReaderType;
  typedef otb::ImageFileWriter<TOutputImage> WriterType;

  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<TInputImage, TOutputImage, TFunctor> FilterType;

  // Instantiating object
  typename FilterType::Pointer object = FilterType::New();
  typename ReaderType::Pointer reader  = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  object->SetInput(reader->GetOutput());
  object->SetRadius(atoi(argv[3]));
  OffsetType offset;
  offset[0] =  atoi(argv[4]);
  offset[1] =  atoi(argv[5]);

  object->SetOffset(offset);
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}

int otbTextureFunctor(int argc, char * argv[])
{
  std::string strArgv(argv[1]);
  argc--;
  argv++;

  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::VectorImage<InputPixelType, Dimension> ImageType;
  typedef ImageType::PixelType                        PixelType;
  typedef itk::ConstNeighborhoodIterator<ImageType>   IteratorType;

  if (strArgv == "ENJ")
    {
    typedef otb::Functor::EnergyTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "ENT")
    {
    typedef otb::Functor::EntropyTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "IMD")
    {
    typedef otb::Functor::InverseDifferenceMomentTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "ASM")
    {
    typedef otb::Functor::AngularSecondMomentumTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "VAR")
    {
    typedef otb::Functor::VarianceTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "COR")
    {
    typedef otb::Functor::CorrelationTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "CON")
    {
    typedef otb::Functor::ContrastTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "SAV")
    {
    typedef otb::Functor::SumAverageTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "DEN")
    {
    typedef otb::Functor::DifferenceEntropyTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "SEN")
    {
    typedef otb::Functor::SumEntropyTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "SVA")
    {
    typedef otb::Functor::SumVarianceTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "DVA")
    {
    typedef otb::Functor::DifferenceVarianceTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "IC1")
    {
    typedef otb::Functor::InformationMeasureOfCorrelation1TextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "IC2")
    {
    typedef otb::Functor::InformationMeasureOfCorrelation2TextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "CSH")
    {
    typedef otb::Functor::ClusterShadeTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "CPR")
    {
    typedef otb::Functor::ClusterProminenceTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else if (strArgv == "MEA")
    {
    typedef otb::Functor::MeanTextureFunctor<InputPixelType, InputPixelType> FunctorType;
    return (generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc, argv));
    }
  else
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
