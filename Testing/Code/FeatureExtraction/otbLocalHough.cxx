/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   11 avril 2006
  Version   :   
  Role      :   Test le filtre local de Hough 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"



#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbLineSpatialObjectList.h"

#include "otbLocalHoughFilter.h"


int otbLocalHough( int argc, char* argv[] )
{
  try 
    {  
    	const char * inputFilename  = argv[1];
    	unsigned int  RadiusX((unsigned int)::atoi(argv[2]));
    	unsigned int  RadiusY((unsigned int)::atoi(argv[3]));
    	unsigned int  NumberOfLines((unsigned int)::atoi(argv[4]));
                       
        typedef unsigned char	                                InputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image< InputPixelType,  Dimension >        InputImageType;

        typedef otb::LocalHoughFilter< InputImageType >   FilterType;
	
        FilterType::Pointer filter = FilterType::New();
        
        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        
        ReaderType::Pointer reader = ReaderType::New();

        reader->SetFileName( inputFilename  );
        reader->Update(); 
        
        FilterType::SizeType Radius;
	Radius[0] = RadiusX;
	Radius[1] = RadiusY;
        
  	typedef otb::LineSpatialObjectList			LinesListType;
  	LinesListType::Pointer list = LinesListType::New();
  	
  	        
	filter->SetRadius( Radius );
	filter->SetNumberOfLines( NumberOfLines );
        
        filter->SetInput( reader->GetOutput() );
        filter->Update();
        
        list = filter->GetOutput();
        
        LinesListType::const_iterator it;
        std::cout<<list->size()<<std::endl;
/*        for (it=list.begin(); it!=list.end(); it++)
           std::cout<< (*it) <<std::endl;*/
       
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


