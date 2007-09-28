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

#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

int otbStreamingImageFileWriterTestCalculateNumberOfDivisions (int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        std::string MethodCalculateNumberOfStreamDivision(argv[3]);

        typedef unsigned int  	                                PixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image< PixelType,  Dimension >        		ImageType;
        typedef otb::ImageFileReader< ImageType  >              ReaderType;
        typedef otb::StreamingImageFileWriter< ImageType>       StreamingWriterType;
        
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName( inputFilename  );

        std::cout << "Streaming writing test"<<std::endl;
        StreamingWriterType::Pointer writer = StreamingWriterType::New();
        writer->SetFileName( outputFilename );
        writer->SetInput( reader->GetOutput() );

        if( MethodCalculateNumberOfStreamDivision == "AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS" )
        {
                writer->SetAutomaticNumberOfStreamDivisions();
        }
        else if( MethodCalculateNumberOfStreamDivision == "NUMBER_OF_STREAM_DIVISIONS" )
        {
                writer->SetNumberOfStreamDivisions(::atoi(argv[4]));
        }
        else if( MethodCalculateNumberOfStreamDivision == "BUFFER_MEMORY_SIZE" )
        {
                writer->SetBufferMemorySize(::atoi(argv[4]));
        }
        else if( MethodCalculateNumberOfStreamDivision == "BUFFER_NUMBER_OF_LINES" )
        {
                writer->SetBufferNumberOfLinesDivisions(::atoi(argv[4]));
        }
				else if( MethodCalculateNumberOfStreamDivision == "TILING_STREAM_DIVISIONS" )
				{
								writer->SetTilingStreamDivisions();
				}
				else if( MethodCalculateNumberOfStreamDivision == "TILING_NUMBER_OF_STREAM_DIVISIONS" )
				{
								std::cout << "Number : " << ::atoi(argv[4]) << std::endl;
								writer->SetTilingStreamDivisions(::atoi(argv[4]));
				} 
        else if( MethodCalculateNumberOfStreamDivision == "DEFAULT" )
        {
        
        }
        else
        {
                itkGenericExceptionMacro(<<"Parameter value not authorized !!!");
        }
        writer->Update(); 

        
  } 
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Exception OTB attrappee dans exception ITK !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( std::bad_alloc & err )
  {
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl;
    return EXIT_FAILURE;
  }
  catch( ... )
  {
    std::cerr << "Exception OTB non attrappee !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
