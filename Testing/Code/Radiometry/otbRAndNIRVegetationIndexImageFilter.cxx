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

#include "otbRAndNIRVegetationIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVegetationIndex.h"

template<class TInputRImage, class TInputNIRImage, class TOutputImage, class TFunction>
int generic_RAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  try
    {

        typedef otb::ImageFileReader<TInputRImage> RReaderType;
        typedef otb::ImageFileReader<TInputNIRImage> NIRReaderType;
        typedef otb::ImageFileWriter<TOutputImage> WriterType;

        typedef otb::RAndNIRVegetationIndexImageFilter<TInputRImage,TInputNIRImage,TOutputImage,TFunction> 
                                                RAndNIRVegetationIndexImageFilterType;

        // Instantiating object
        typename RAndNIRVegetationIndexImageFilterType::Pointer filter = RAndNIRVegetationIndexImageFilterType::New();
        typename RReaderType::Pointer readerR = RReaderType::New();
        typename NIRReaderType::Pointer readerNIR = NIRReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();

        const char * inputFilenameR  = argv[1];
        const char * inputFilenameNIR  = argv[2];
        const char * outputFilename = argv[3];

        readerR->SetFileName( inputFilenameR );
        readerNIR->SetFileName( inputFilenameNIR );
        writer->SetFileName( outputFilename  );
        filter->SetInputR( readerR->GetOutput() ); 
        filter->SetInputNIR( readerNIR->GetOutput() ); 
        writer->SetInput( filter->GetOutput() );
        writer->Update();

    }

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;


}

int otbRAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
        const unsigned int Dimension = 2;
        typedef unsigned char PixelType;
        typedef otb::Image<PixelType,Dimension> InputRImageType;
        typedef otb::Image<PixelType,Dimension> InputNIRImageType;
        typedef otb::Image<float,Dimension> OutputImageType;
        
        std::string strArgv(argv[1]);
        argc--;
        argv++;
	if ( strArgv == "NDVI" ) return( generic_RAndNIRVegetationIndexImageFilter<InputRImageType, InputNIRImageType, OutputImageType,
                                                otb::Functor::NDVI<     InputRImageType::PixelType,
                                                                        InputNIRImageType::PixelType,
                                                                        OutputImageType::PixelType> >
                                                                        (argc,argv) );
	else if ( strArgv == "RVI" ) return( generic_RAndNIRVegetationIndexImageFilter<InputRImageType, InputNIRImageType, OutputImageType,
                                                otb::Functor::RVI<     InputRImageType::PixelType,
                                                                        InputNIRImageType::PixelType,
                                                                        OutputImageType::PixelType> >
                                                                        (argc,argv) );
/*	else if ( strArgv == "PVI" ) return( generic_RAndNIRVegetationIndexImageFilter<InputRImageType, InputNIRImageType, OutputImageType,
                                                otb::Functor::PVI<     InputRImageType::PixelType,
                                                                        InputNIRImageType::PixelType,
                                                                        OutputImageType::PixelType> >
                                                                        (argc,argv) );*/
	else if ( strArgv == "SAVI" ) return( generic_RAndNIRVegetationIndexImageFilter<InputRImageType, InputNIRImageType, OutputImageType,
                                                otb::Functor::SAVI<     InputRImageType::PixelType,
                                                                        InputNIRImageType::PixelType,
                                                                        OutputImageType::PixelType> >
                                                                        (argc,argv) );
/*	else if ( strArgv == "TSAVI" ) return( generic_RAndNIRVegetationIndexImageFilter<InputRImageType, InputNIRImageType, OutputImageType,
                                                otb::Functor::TSAVI<     InputRImageType::PixelType,
                                                                        InputNIRImageType::PixelType,
                                                                        OutputImageType::PixelType> >
                                                                        (argc,argv) );*/
	else if ( strArgv == "MSAVI" ) return( generic_RAndNIRVegetationIndexImageFilter<InputRImageType, InputNIRImageType, OutputImageType,
                                                otb::Functor::MSAVI<     InputRImageType::PixelType,
                                                                        InputNIRImageType::PixelType,
                                                                        OutputImageType::PixelType> >
                                                                        (argc,argv) );
        else
                return EXIT_FAILURE;
        return EXIT_SUCCESS;
}
