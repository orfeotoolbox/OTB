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

//#define MAIN


#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkExceptionObject.h"
#include <iostream>
#include "itkComplexToModulusImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"

int otbImageFileReaderONERAComplex(int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];

        typedef std::complex<float>  	                        InputPixelType;
        typedef float                             		OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
        typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef itk::StreamingImageFilter< InputImageType, InputImageType >  StreamingType;
	
        StreamingType::Pointer streaming = StreamingType::New();
        ReaderType::Pointer complexReader = ReaderType::New();
 
	complexReader->SetFileName( inputFilename  );
	streaming->SetNumberOfStreamDivisions(100);
	streaming->SetInput(complexReader->GetOutput());

  	typedef itk::ComplexToModulusImageFilter< 
                       InputImageType, OutputImageType > ModulusFilterType;

  	ModulusFilterType::Pointer modulusFilter = ModulusFilterType::New();
  	modulusFilter->SetInput( streaming->GetOutput() );

        typedef otb::ExtractROI< OutputPixelType, 
                                 OutputPixelType >  ExtractROIFilterType;

        ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

	extractROIFilter->SetStartX( 10 );
	extractROIFilter->SetStartY( 10 );
	extractROIFilter->SetSizeX( 100 );
	extractROIFilter->SetSizeY( 100 );
        extractROIFilter->SetInput( modulusFilter->GetOutput() );        

        WriterType::Pointer writer = WriterType::New();
	
        writer->SetFileName( outputFilename );        
        writer->SetInput( modulusFilter->GetOutput() );
        writer->Update(); 
  } 
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Catch ITK Exception in OTB " << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... )
  {
    std::cerr << "catch OTB Exception  !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

