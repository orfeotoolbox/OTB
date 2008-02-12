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
#include "itkObject.h"

#include "otbMacro.h"

#include "otbShapeFileDataReader.h"
#include "otbShapeFileDataWriter.h"

#include "ogrsf_frmts.h"

#include <iostream>


int otbShapeFileDataWriterTest(int argc, char* argv[])
{
  try
  {
    // Verify the number of parameters in the command line
    const char * inputFilename  = argv[1];
		const char * outputFilename = argv[2];

		typedef otb::ShapeFileData		ShapeFileDataType;
		typedef otb::ShapeFileDataReader<ShapeFileDataType> ShapeFileDataReaderType;
		typedef otb::ShapeFileDataWriter<ShapeFileDataType> ShapeFileDataWriterType;
		
		ShapeFileDataType* SHPdata ;
		ShapeFileDataReaderType::Pointer reader = ShapeFileDataReaderType::New();
		ShapeFileDataWriterType::Pointer writer = ShapeFileDataWriterType::New();
		
		reader->SetFileName(inputFilename);
		writer->SetInput(reader->GetOutput());
		writer->SetFileName(outputFilename);
		writer->Update();
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
