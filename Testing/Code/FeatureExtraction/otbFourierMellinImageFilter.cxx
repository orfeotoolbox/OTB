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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbForwardFourierMellinTransformImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"

int otbFourierMellinImageFilter(int argc, char* argv[])
{
  try 
    {

        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        typedef double                                          InputPixelType;
        typedef std::complex<double>                            OutputPixelType;
  	const   unsigned int        	                        Dimension = 2;

  	typedef otb::Image< InputPixelType, Dimension >         InputImageType;
  	typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;
   
  	typedef itk::LinearInterpolateImageFunction< InputImageType, double >	InterpolatorType;
        typedef otb::ForwardFourierMellinTransformImageFilter<InputPixelType,
  				InterpolatorType,Dimension> 			FourierMellinTransformType;
     
     	FourierMellinTransformType::Pointer FourierMellinTransform = FourierMellinTransformType::New();

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );

	OutputImageType::SizeType size;
	size[0]=512;
	size[1]=256;
	
	FourierMellinTransform->SetInput( reader->GetOutput() );
	FourierMellinTransform->SetOutputSize(size);
        writer->SetInput( FourierMellinTransform->GetOutput() );
        
        writer->Update(); 
	
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "itk::Exception detected: "  << err.GetDescription();
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "unknown exception detected !" << std::endl; 
    return EXIT_FAILURE;
    } 
  
  return EXIT_SUCCESS;
}
