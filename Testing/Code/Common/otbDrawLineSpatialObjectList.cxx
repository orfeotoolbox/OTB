/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   10 avril 2006
  Version   :   
  Role      :   Test le filtre DrawLineSpatialOjectListImageFilter 
  	    :   qui permet de dessiner les lignes contenues dans une liste 
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

#include "otbDrawLineSpatialObjectListFilter.h"


int otbDrawLineSpatialObjectList( int argc, char* argv[] )
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

        typedef otb::DrawLineSpatialObjectListFilter< InputImageType,OutputImageType >   FilterType;
	
        FilterType::Pointer filter = FilterType::New();
        
        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
  	typedef otb::LineSpatialObjectList			LinesListType;
  	typedef LinesListType::LineType	LineType;
  	LinesListType list;
  	
  	LineType::PointListType pointList;
        LineType::LinePointType point;
        
        // Definition of the first line
        double Ux, Uy, Vx, Vy;
        Ux = 10.;
        Uy = 12.;
        Vx = 35.;
        Vy = 29.;
        
        point.SetPosition(Ux,Uy);
        pointList.push_back(point);
        point.SetPosition(Vx,Vy);
        pointList.push_back(point);
        
        LineType::Pointer line = LineType::New();
        line->SetId(0);
        line->SetPoints( pointList );
        line->ComputeBoundingBox();
       
        list.push_back(line);
        
        // Definition of a second line
        Ux = 8.;
        Uy = 7.;
        Vx = 8.;
        Vy = 46.;
        
        point.SetPosition(Ux,Uy);
        pointList.push_back(point);
        point.SetPosition(Vx,Vy);
        pointList.push_back(point);
        
        LineType::Pointer line2 = LineType::New();
        line2->SetId(0);
        line2->SetPoints( pointList );
        line2->ComputeBoundingBox();
       
        list.push_back(line2); 
        
        filter->SetInputLineSpatialObjectList(list);
        
        filter->SetInput( reader->GetOutput() );
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


