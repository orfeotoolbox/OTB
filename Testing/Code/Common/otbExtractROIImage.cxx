/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test l'extraction d'une ROI dans une image mono canal

=========================================================================*/

#include "itkExceptionObject.h"


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
//#include "itkRegionOfInterestImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkImage.h"


int otbExtractROIImage( int argc, char ** argv )
{
        // Verify the number of parameters in the command line
/*        if( argc != 7 )
        {
                std::cerr << "Usage: " << argv[0] << " inputImageFile  outputImageFile " << std::endl;
                std::cerr << " startX startY sizeX sizeY" << std::endl;
                return -1;
        }*/
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        int  startX(::atoi(argv[3]));
        int  startY(::atoi(argv[4]));
        int  sizeX(::atoi(argv[5]));
        int  sizeY(::atoi(argv[6]));

        typedef unsigned char  	                                InputPixelType;
        typedef unsigned char  	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef itk::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;
/*  typedef itk::RegionOfInterestImageFilter< InputImageType, 
                                            OutputImageType > FilterType;*/
        typedef itk::ExtractImageFilter< InputImageType, 
                                            OutputImageType >   FilterType;
        /* typename */FilterType::Pointer filter = FilterType::New();


        /* typename */ OutputImageType::IndexType start;
        start[0] = startX;
        start[1] = startY;
        /* typename */ OutputImageType::SizeType size;
        size[0] = sizeX;
        size[1] = sizeY;
        /* typename */ OutputImageType::RegionType desiredRegion;
        desiredRegion.SetSize(  size  );
        desiredRegion.SetIndex( start );
        
        filter->SetExtractionRegion( desiredRegion );

        /* typename */ ReaderType::Pointer reader = ReaderType::New();
        /* typename */ WriterType::Pointer writer = WriterType::New();
        


        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        filter->SetInput( reader->GetOutput() );
        writer->SetInput( filter->GetOutput() );
  try 
    { 
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


  return EXIT_SUCCESS;
}


