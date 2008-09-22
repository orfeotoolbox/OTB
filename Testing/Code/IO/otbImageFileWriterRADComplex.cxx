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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"

int otbImageFileWriterRADComplex(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef std::complex<int>  	                        PixelType;
  const   unsigned int        	                        Dimension = 2;

  typedef otb::Image< PixelType,  Dimension >        ImageType;

  typedef otb::ImageFileReader< ImageType  >         ReaderType;
  typedef otb::ImageFileWriter< ImageType >         WriterType;

  ReaderType::Pointer complexReader = ReaderType::New();
  WriterType::Pointer complexWriter = WriterType::New();
   
  complexReader->SetFileName( inputFilename  );
  complexWriter->SetFileName( outputFilename  );
  complexWriter->SetInput( complexReader->GetOutput()  );
  complexWriter->Update();


  
  return EXIT_SUCCESS;
}

