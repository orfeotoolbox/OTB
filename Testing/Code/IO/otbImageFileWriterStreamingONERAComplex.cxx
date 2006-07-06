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

#include "otbImage.h"
#include "itkExceptionObject.h"
#include <iostream>
#include "itkComplexToModulusImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

int otbImageFileWriterStreamingONERAComplex(int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];

        typedef std::complex<float>  	                        PixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image<PixelType,  Dimension >              ImageType;

        typedef otb::ImageFileReader< ImageType  >         ReaderType;
        typedef otb::StreamingImageFileWriter< ImageType >          WriterType;
/*        typedef itk::StreamingImageFilter< ImageType, 
					   ImageType >       StreamingType;*/

        ReaderType::Pointer complexReader = ReaderType::New(); 
	complexReader->SetFileName( inputFilename  );

/*        StreamingType::Pointer streaming = StreamingType::New();
	streaming->SetNumberOfStreamDivisions(100);
	streaming->SetInput(complexReader->GetOutput());
*/ 
        WriterType::Pointer complexWriter = WriterType::New();
	complexWriter->SetNumberOfStreamDivisions(100);
	complexWriter->SetFileName( outputFilename  );
	complexWriter->SetInput( complexReader->GetOutput()  );
 	complexWriter->Update();

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

