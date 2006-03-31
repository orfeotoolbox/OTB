/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   14 mars 2006
  Version   :   
  Role      :   Test du filtre de suppression par direction des pixels isoles 
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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPixelSuppressionByDirectionImageFilter.h"

int otbPixelSuppressionByDirection( int argc, char* argv[] )
{
  try 
    { 
        const char * inputFilename1  = argv[1];
        const char * inputFilename2  = argv[2];
        const char * outputFilename = argv[3];

	unsigned int  RadiusX((unsigned int)::atoi(argv[3]));
	double	      AngularBeam((double)::atof(argv[4]));
        
        typedef double		                                InputPixelType;
        typedef double		   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType1;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType1  >         ReaderType1;
        typedef otb::ImageFileReader< InputImageType1  >         ReaderType2;
        typedef otb::ImageFileWriter< OutputImageType >          WriterType;
        
        typedef otb::PixelSuppressionByDirection< InputImageType1, OutputImageType >   FilterType;

        FilterType::Pointer filter = FilterType::New();
                
                
	FilterType::SizeType Radius;
	Radius[0] = RadiusX;
	Radius[1] = RadiusX;
	

        filter->SetRadius( Radius );
        filter->SetAngularBeam( static_cast<FilterType::InputRealType>( AngularBeam ));
	
	
        ReaderType1::Pointer reader1 = ReaderType1::New();
        ReaderType2::Pointer reader2 = ReaderType2::New();
        WriterType::Pointer writer = WriterType::New();

        reader1->SetFileName( inputFilename1  );
        reader2->SetFileName( inputFilename2  );
        writer->SetFileName( outputFilename );
        
        filter->SetInputImage( reader1->GetOutput() );
        filter->SetInputImageDirection( reader2->GetOutput() );
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


