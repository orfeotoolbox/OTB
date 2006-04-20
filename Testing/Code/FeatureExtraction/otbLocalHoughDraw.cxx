/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   10 avril 2006
  Version   :   
  Role      :   Test le filtre Local Hough en dessinant les lignes 
  	    :   dans une image 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"

#include <iostream>
#include <list>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbLineSpatialObjectList.h"

#include "otbLocalHoughFilter.h"
#include "otbDrawLineSpatialObjectListFilter.h"

#include "otbImageToLineSpatialObjectListFilter.h"


int otbLocalHoughDraw( int argc, char* argv[] )
{
  try 
    {  
    	const char * inputFilename  = argv[1];
    	const char * outputFilename = argv[2];
    	
    	unsigned int  RadiusX((unsigned int)::atoi(argv[3]));
    	unsigned int  RadiusY((unsigned int)::atoi(argv[4]));
    	unsigned int  NumberOfLines((unsigned int)::atoi(argv[5]));
                       
        typedef unsigned char	                                InputPixelType;
        typedef unsigned char	   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::LocalHoughFilter< InputImageType >   FilterType;
//typedef otb::ImageToLineSpatialObjectListFilter< InputImageType >   FilterType;
	
        FilterType::Pointer filter = FilterType::New();
        
        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;
        
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        reader->Update();
        
        writer->SetFileName( outputFilename ); 
        
        FilterType::SizeType Radius;
	Radius[0] = RadiusX;
	Radius[1] = RadiusY;
        
                
	filter->SetRadius( Radius );
	filter->SetNumberOfLines( NumberOfLines );
     
        filter->SetInput( reader->GetOutput() ); 
        filter->Update();
       

        typedef otb::DrawLineSpatialObjectListFilter< InputImageType,OutputImageType >   DrawFilterType;
	
        DrawFilterType::Pointer drawfilter = DrawFilterType::New();
        
       
        drawfilter->SetInputLineSpatialObjectList(filter->GetOutput());
        
        drawfilter->SetInput( reader->GetOutput() );
        writer->SetInput( drawfilter->GetOutput() );
        
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


