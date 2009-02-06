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

#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbVectorImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

// Functors list
#include "otbEnergyTextureFunctor.h"
#include "otbEntropyTextureFunctor.h"
#include "otbInverseDifferenceMomentTextureFunctor.h"
#include "otbAngularSecondMomentumTextureFunctor.h"
#include "otbVarianceTextureFunctor.h"
#include "otbCorrelationTextureFunctor.h"
#include "otbContrastTextureFunctor.h"


template<class TInputImage, class TOutputImage, class TFunctor>
int generic_TextureFunctor(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef typename TInputImage::SizeType SizeType;
  typedef typename TInputImage::OffsetType OffsetType;
  typedef otb::ImageFileReader<TInputImage>  ReaderType;
  typedef otb::StreamingImageFileWriter<TOutputImage> WriterType;
  
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
  typedef otb::VectorImage<InputPixelType,Dimension> ImageType;
  typedef ImageType::PixelType                       PixelType;
  typedef itk::ConstNeighborhoodIterator<ImageType>  IteratorType;

  if(strArgv == "ENJ")
    {
      typedef otb::Functor::EnergyTextureFunctor<IteratorType, IteratorType, PixelType> FunctorType;
      return( generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc,argv) );
    }
  else if ( strArgv == "ENT" )
    {
      typedef otb::Functor::EntropyTextureFunctor<IteratorType, IteratorType, PixelType> FunctorType;
      return( generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc,argv) );
    }
  else if ( strArgv == "IMD" )
    {
      typedef otb::Functor::InverseDifferenceMomentTextureFunctor<IteratorType, IteratorType, PixelType> FunctorType;
      return( generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc,argv) );
    }
 else if ( strArgv == "ASM" )
    {
      typedef otb::Functor::AngularSecondMomentumTextureFunctor<IteratorType, IteratorType, PixelType> FunctorType;
      return( generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc,argv) );
    }
 else if ( strArgv == "VAR" )
    {
      typedef otb::Functor::VarianceTextureFunctor<IteratorType, IteratorType, PixelType> FunctorType;
      return( generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc,argv) );
    }
 else if ( strArgv == "COR" )
    {
      typedef otb::Functor::CorrelationTextureFunctor<IteratorType, IteratorType, PixelType> FunctorType;
      return( generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc,argv) );
    }
 else if ( strArgv == "CON" )
    {
      typedef otb::Functor::ContrastTextureFunctor<IteratorType, IteratorType, PixelType> FunctorType;
      return( generic_TextureFunctor<ImageType, ImageType, FunctorType>(argc,argv) );
    }
  else
    {
      return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}
