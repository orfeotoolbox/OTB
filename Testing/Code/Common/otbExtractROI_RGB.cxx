/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test l'extraction d'une ROI dans une image RGB

=========================================================================*/

#include "itkExceptionObject.h"


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include "itkImage.h"
#include "itkRGBPixel.h"


int otbExtractROIImageRGB( int argc, char ** argv )
{
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        int  startX(::atoi(argv[3]));
        int  startY(::atoi(argv[4]));
        int  sizeX(::atoi(argv[5]));
        int  sizeY(::atoi(argv[6]));

        typedef itk::RGBPixel<unsigned char>                    InputPixelType;
        typedef itk::RGBPixel<unsigned char>                    OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef itk::ImageFileReader< InputImageType  >         ReaderType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;
        typedef itk::ExtractImageFilter< InputImageType, 
                                            OutputImageType >   FilterType;
        FilterType::Pointer filter = FilterType::New();


        OutputImageType::IndexType start;
        start[0] = startX;
        start[1] = startY;
        OutputImageType::SizeType size;
        size[0] = sizeX;
        size[1] = sizeY;
        OutputImageType::RegionType desiredRegion;
        desiredRegion.SetSize(  size  );
        desiredRegion.SetIndex( start );
        
        filter->SetExtractionRegion( desiredRegion );

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

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


