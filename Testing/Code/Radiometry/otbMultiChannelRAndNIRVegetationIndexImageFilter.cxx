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

#include "otbMultiChannelRAndNIRVegetationIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVegetationIndex.h"


template<class TInputImage, class TOutputImage, class TFunction>
int generic_MultiChannelRAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  typedef otb::ImageFileReader<TInputImage> ReaderType;
  typedef otb::ImageFileWriter<TOutputImage> WriterType;
  
  typedef otb::MultiChannelRAndNIRVegetationIndexImageFilter<TInputImage,TOutputImage,TFunction> 
    MultiChannelRAndNIRVegetationIndexImageFilterType;
  
  // Instantiating object
  typename MultiChannelRAndNIRVegetationIndexImageFilterType::Pointer filter = MultiChannelRAndNIRVegetationIndexImageFilterType::New();
  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();
  
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  unsigned int redChannel(::atoi(argv[3]));
  unsigned int nirChannel(::atoi(argv[4]));
  reader->SetFileName( inputFilename );
  writer->SetFileName( outputFilename  );
  filter->SetRedIndex(redChannel);
  filter->SetNIRIndex(nirChannel);
  filter->SetInput( reader->GetOutput() ); 
  writer->SetInput( filter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}

int otbMultiChannelRAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<unsigned char ,Dimension> InputImageType;
  typedef otb::Image<float,Dimension> OutputImageType;
        
  std::string strArgv(argv[1]);
  argc--;
  argv++;
  if ( strArgv == "NDVI" ) return( generic_MultiChannelRAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
				   otb::Functor::NDVI<     InputImageType::InternalPixelType,
				   InputImageType::InternalPixelType,
				   OutputImageType::PixelType> >
				   (argc,argv) );
  else if ( strArgv == "RVI" ) return( generic_MultiChannelRAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
				       otb::Functor::RVI<     InputImageType::InternalPixelType,
				       InputImageType::InternalPixelType,
				       OutputImageType::PixelType> >
				       (argc,argv) );
  /*	else if ( strArgv == "PVI" ) return( generic_MultiChannelRAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
	otb::Functor::PVI<     InputImageType::InternalPixelType,
	InputImageType::InternalPixelType,
	OutputImageType::PixelType> >
	(argc,argv) );*/
  else if ( strArgv == "SAVI" ) return( generic_MultiChannelRAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
					otb::Functor::SAVI<     InputImageType::InternalPixelType,
					InputImageType::InternalPixelType,
					OutputImageType::PixelType> >
					(argc,argv) );
  /*	else if ( strArgv == "TSAVI" ) return( generic_MultiChannelRAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
	otb::Functor::TSAVI<     InputImageType::InternalPixelType,
	InputImageType::InternalPixelType,
	OutputImageType::PixelType> >
	(argc,argv) );*/
  else if ( strArgv == "MSAVI" ) return( generic_MultiChannelRAndNIRVegetationIndexImageFilter<InputImageType, OutputImageType,
					 otb::Functor::MSAVI<    InputImageType::InternalPixelType,
					 InputImageType::InternalPixelType,
					 OutputImageType::PixelType> >
					 (argc,argv) );
  else
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
