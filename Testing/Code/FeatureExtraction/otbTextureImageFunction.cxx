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
#include "itkExceptionObject.h"

#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "otbTextureImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkVariableLengthVector.h"


// Functors list
#include "otbEnergyTextureFunctor.h"
#include "otbEntropyTextureFunctor.h"
#include "otbInverseDifferenceMomentTextureFunctor.h"
#include "otbAngularSecondMomentumTextureFunctor.h"
#include "otbVarianceTextureFunctor.h"
#include "otbCorrelationTextureFunctor.h"
#include "otbContrastTextureFunctor.h"
#include "otbSumAverageTextureFunctor.h"


template<class TInputImage, class TOutputImage, class TFunctor>
int generic_TextureImageFunction(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef typename TInputImage::SizeType SizeType;
  typedef typename TInputImage::OffsetType OffsetType;
  typedef otb::ImageFileReader<TInputImage>  ReaderType;
  typedef otb::StreamingImageFileWriter<TOutputImage> WriterType;

  typedef otb::TextureImageFunction<TInputImage, TFunctor> FunctionType;
  typedef otb::FunctionWithNeighborhoodToImageFilter<TInputImage, TOutputImage, FunctionType> FilterType;

  typename FunctionType::Pointer energyFunction = FunctionType::New();
  typename FilterType::Pointer filter = FilterType::New();


  // Instantiating object
  typename ReaderType::Pointer reader  = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  filter->SetInput(reader->GetOutput());

  SizeType radius;
  radius[0] = atoi(argv[3]);
  radius[1] = atoi(argv[4]);
  energyFunction->SetRadius(radius);
  OffsetType offset;
  offset[0] =  atoi(argv[5]);
  offset[1] =  atoi(argv[6]);
  energyFunction->SetOffset(offset);

  filter->SetFunction(energyFunction);
  writer->SetInput(filter->GetOutput());

  writer->Update();


  return EXIT_SUCCESS;
}

int otbTextureImageFunction(int argc, char * argv[])
{
  std::string strArgv(argv[1]);
  argc--;
  argv++;
 

  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> ImageType;
  typedef itk::VariableLengthVector<double> VectorType;
  typedef itk::ConstNeighborhoodIterator<ImageType> IteratorType;
 

  if(strArgv == "ENJ")
    {
      std::cout<<"ENEGRY"<<std::endl;
      typedef otb::Functor::EnergyTextureFunctor<IteratorType, IteratorType, VectorType> FunctorType;
      return( generic_TextureImageFunction<ImageType, ImageType, FunctorType>(argc,argv) );
    }
  else if ( strArgv == "ENT" )
    {
      typedef otb::Functor::EntropyTextureFunctor<IteratorType, IteratorType, VectorType> FunctorType;
      return( generic_TextureImageFunction<ImageType, ImageType, FunctorType>(argc,argv) );
    }
  else if ( strArgv == "IMD" )
    {
      typedef otb::Functor::InverseDifferenceMomentTextureFunctor<IteratorType, IteratorType, VectorType> FunctorType;
      return( generic_TextureImageFunction<ImageType, ImageType, FunctorType>(argc,argv) );
    }
 else if ( strArgv == "ASM" )
    {
      typedef otb::Functor::AngularSecondMomentumTextureFunctor<IteratorType, IteratorType, VectorType> FunctorType;
      return( generic_TextureImageFunction<ImageType, ImageType, FunctorType>(argc,argv) );
    }
 else if ( strArgv == "VAR" )
    {
      typedef otb::Functor::VarianceTextureFunctor<IteratorType, IteratorType, VectorType> FunctorType;
      return( generic_TextureImageFunction<ImageType, ImageType, FunctorType>(argc,argv) );
    }
 else if ( strArgv == "COR" )
    {
      typedef otb::Functor::CorrelationTextureFunctor<IteratorType, IteratorType, VectorType> FunctorType;
      return( generic_TextureImageFunction<ImageType, ImageType, FunctorType>(argc,argv) );
    }
  else if ( strArgv == "CON" )
    {
      typedef otb::Functor::ContrastTextureFunctor<IteratorType, IteratorType, VectorType> FunctorType;
      return( generic_TextureImageFunction<ImageType, ImageType, FunctorType>(argc,argv) );
    }
  else if ( strArgv == "SAV" )
    {
      typedef otb::Functor::SumAverageTextureFunctor<IteratorType, IteratorType, VectorType> FunctorType;
      return( generic_TextureImageFunction<ImageType, ImageType, FunctorType>(argc,argv) );
    }
  else
    {
      return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}
