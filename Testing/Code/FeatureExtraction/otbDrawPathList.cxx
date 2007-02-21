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
#include "itkPolyLineParametricPath.h"

#include "otbImage.h"
#include "otbPathListSource.h"
#include "otbImageToPathListAlignFilter.h"
#include "otbDrawPathListFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbDrawPathList( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       

        typedef unsigned char                                   InputPixelType;
        typedef unsigned char   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image< InputPixelType,  Dimension >	InputImageType;
        typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

	typedef itk::PolyLineParametricPath< Dimension >	PathType;
	typedef PathType::Pointer                               PathPointerType;	

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;  
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;
	
	typedef otb::DrawPathListFilter<InputImageType,PathType,OutputImageType> DrawPathListFilterType;

        typedef otb::ImageToPathListAlignFilter<InputImageType,PathType>  PathListType;

        ReaderType::Pointer reader         = ReaderType::New();
        WriterType::Pointer writer         = WriterType::New();
	
	DrawPathListFilterType::Pointer DrawPath = DrawPathListFilterType::New();

	PathListType::Pointer testList = PathListType::New();
	
        reader->SetFileName( inputFilename  );
	writer->SetFileName( outputFilename );
	
//OTB-FA-00010-CS
	testList->SetInput( reader->GetOutput() );
	testList->Update();
	
	DrawPath->SetImageInput(reader->GetOutput()  );
	DrawPath->SetPathInput( testList->GetOutput());
	DrawPath->Update();
	
	writer->SetInput(DrawPath->GetImageOutput());
        writer->Update(); 

    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}
