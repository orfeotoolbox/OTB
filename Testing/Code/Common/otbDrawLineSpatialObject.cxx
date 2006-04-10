/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   06 avril 2006
  Version   :   
  Role      :   Test du filtre DrawLineSpatialOjectImageFilter 
  	    :   qui permet de dessiner une ligne dans une image 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkLineSpatialObject.h"
#include <iostream>
#include <list>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbDrawLineSpatialObjectFilter.h"


int otbDrawLineSpatialObject( int argc, char* argv[] )
{
  try 
    { 
    	const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
                
        typedef double		                                InputPixelType;
        typedef unsigned char	   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::DrawLineSpatialObjectFilter< InputImageType,OutputImageType >   FilterType;
	
        FilterType::Pointer filter = FilterType::New();
        
        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        typedef itk::LineSpatialObject<2>	LineType;
        LineType::PointListType list;
        LineType::LinePointType point;
        
        double Ux, Uy, Vx, Vy;
        Ux = 10.;
        Uy = 12.;
        Vx = 35.;
        Vy = 29.;
        
        point.SetPosition(Ux,Uy);
        list.push_back(point);
        point.SetPosition(Vx,Vy);
        list.push_back(point);
        
        LineType::Pointer line = LineType::New();
        line->SetId(0);
        line->SetPoints( list );
        line->ComputeBoundingBox();
       
        
        filter->SetValue( 245 );
        filter->SetInput( line );
        
        filter->SetInputImage( reader->GetOutput() );
        writer->SetInput( filter->GetOutput() );
        
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


