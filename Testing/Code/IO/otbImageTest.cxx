/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   27 mars 2006
  Version   :   
  Role      :   Test d'instanciation de la classe otb::Image
  		Lecture de meta données
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif



#include "itkExceptionObject.h"
#include <iostream>
#include <string>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbMetaDataKey.h"


int otbImageTest(int argc, char* argv[])
{
  try
  {
        // Verify the number of parameters in the command line
        const char * inputFilename  = argv[1];
      //  unsigned int  GCPnum((unsigned int)::atoi(argv[2]));

        typedef unsigned char  	                                InputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
        
        InputImageType::Pointer image = InputImageType::New();
        
        typedef otb::ImageFileReader< InputImageType >         ReaderType;
        

        ReaderType::Pointer reader = ReaderType::New();
 
        reader->SetFileName( inputFilename  );
	reader->Update();

	image = reader->GetOutput();
	
	std::cout << "------ IMAGE --------" << std::endl;
	std::cout << image << std::endl;	
	std::cout << "---------------------" << std::endl;
	
	std::cout << "Spacing " << image->GetSpacing() << std::endl;
	std::cout << "Origin " << image->GetOrigin() << std::endl;
	
	std::cout << "Projection REF " << image->GetProjectionRef() << std::endl;
		 
	std::cout << "GCP Projection " << image->GetGCPProjection() << std::endl;
	
	unsigned int GCPCount = image->GetGCPCount();	
	std::cout << "GCP Count " << image->GetGCPCount() << std::endl; 
	
	for(unsigned int GCPnum = 0 ; GCPnum < GCPCount ; GCPnum++ )
	{
		std::cout << "GCP[" << GCPnum << "] Id " << image->GetGCPId(GCPnum) << std::endl;
		std::cout << "GCP[" << GCPnum << "] Info " << image->GetGCPInfo(GCPnum) << std::endl;
		std::cout << "GCP[" << GCPnum << "] Row " << image->GetGCPRow(GCPnum) << std::endl;
		std::cout << "GCP[" << GCPnum << "] Col " << image->GetGCPCol(GCPnum) << std::endl;
		std::cout << "GCP[" << GCPnum << "] X " << image->GetGCPX(GCPnum) << std::endl;
		std::cout << "GCP[" << GCPnum << "] Y " << image->GetGCPY(GCPnum) << std::endl;
		std::cout << "GCP[" << GCPnum << "] Z " << image->GetGCPZ(GCPnum) << std::endl;
		std::cout << "----------------" << std::endl;
	}
	
	InputImageType::VectorType tab = image->GetGeoTransform();

	std::cout << "Geo Transform " << std::endl;
	for(int i = 0 ; i < tab.size() ; i++ )
	{
		std::cout << " " <<i<<" -> "<<tab[i]<< std::endl;
	}
	tab.clear();
	
	tab = image->GetUpperLeftCorner();
	std::cout << "Corners " << std::endl;
	for(int i = 0 ; i < tab.size() ; i++ )
	{
		std::cout << " UL[" <<i<<"] -> "<<tab[i]<< std::endl;
	}
	tab.clear();
		
	tab = image->GetUpperRightCorner();
	for(int i = 0 ; i < tab.size() ; i++ )
	{
		std::cout << " UR[" <<i<<"] -> "<<tab[i]<< std::endl;
	}
	tab.clear();
		
	tab = image->GetLowerLeftCorner();
	for(int i = 0 ; i < tab.size() ; i++ )
	{
		std::cout << " LL[" <<i<<"] -> "<<tab[i]<< std::endl;
	}
	tab.clear();
			
	tab = image->GetLowerRightCorner();
	for(int i = 0 ; i < tab.size() ; i++ )
	{
		std::cout << " LR[" <<i<<"] -> "<<tab[i]<< std::endl;
	}	
	tab.clear();
		
	
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

