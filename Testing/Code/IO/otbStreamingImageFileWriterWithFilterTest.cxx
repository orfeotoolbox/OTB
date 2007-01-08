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

#include "itkExceptionObject.h"
#include <iostream>

#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "itkMeanImageFilter.h"


int otbStreamingImageFileWriterWithFilterTest (int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
	unsigned int radius = atoi(argv[3]);
	int   iStreaming(::atoi(argv[4]));
        bool streaming = (bool)(iStreaming);
        int NumberOfStreamDivisions(10);
        if( streaming == true )
        {
                NumberOfStreamDivisions = ::atoi(argv[5]);
        }
       
        

        typedef unsigned char  	                                InputPixelType;
        typedef unsigned char  	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::StreamingImageFileWriter< OutputImageType> StreamingWriterType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;
	typedef itk::MeanImageFilter<InputImageType,OutputImageType> FilterType;
        
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName( inputFilename  );
	FilterType::Pointer filter = FilterType::New();
	filter->SetInput(reader->GetOutput());
	InputImageType::SizeType rad;
	rad.Fill(radius);
	filter->SetRadius(rad);

        if( streaming == true )
        {
                std::cout << "Streaming writing test"<<std::endl;
                StreamingWriterType::Pointer writer = StreamingWriterType::New();
                writer->SetFileName( outputFilename );
                writer->SetNumberOfStreamDivisions( NumberOfStreamDivisions );
                writer->SetInput( filter->GetOutput() );
                writer->Update(); 
        }
        else
        {
                std::cout << "Writing test"<<std::endl;
                WriterType::Pointer writer = WriterType::New();
                writer->SetFileName( outputFilename );
                writer->SetInput( reader->GetOutput() );
                writer->Update(); 
        }

        
  } 
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Exception OTB attrappee dans exception ITK !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... )
  {
    std::cerr << "Exception OTB non attrappee !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
