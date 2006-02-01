/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test l'extraction d'une ROI et pour un canal dans une image multi-canal, dont les valeurs sont codées en "unsigned char"
  $Id$

=========================================================================*/

#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbExtractROI.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorToScalarImageFilter.h"

int otbExtractROI_VectorToScalar ( int argc, char ** argv )
{

  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        
        unsigned int  startX((unsigned int)::atoi(argv[3]));
        unsigned int  startY((unsigned int)::atoi(argv[4]));
        unsigned int  sizeX((unsigned int)::atoi(argv[5]));
        unsigned int  sizeY((unsigned int)::atoi(argv[6]));

        unsigned int  channel((unsigned int)::atoi(argv[7]));

        typedef unsigned char  	                                InputPixelType;
        typedef unsigned char  	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;


        typedef otb::VectorToScalarImageFilter< InputPixelType, 
                                             OutputPixelType >   ExtractChannelFilterType;
        typedef otb::ExtractROI< InputPixelType, 
                                            OutputPixelType >    ExtractROIFilterType;

        typedef otb::ImageFileReader< ExtractChannelFilterType::InputImageType, itk::DefaultConvertPixelTraits< InputPixelType >  >       ReaderType;
        typedef otb::ImageFileWriter< ExtractROIFilterType::OutputImageType >           WriterType;

        ExtractChannelFilterType::Pointer extractChannelFilter = ExtractChannelFilterType::New();
        ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
        
        extractROIFilter->SetStartX( startX );
        extractROIFilter->SetStartY( startY );
        extractROIFilter->SetSizeX( sizeX );
        extractROIFilter->SetSizeY( sizeY );

        extractChannelFilter->SetChannel(channel);

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        extractChannelFilter->SetInput( reader->GetOutput() );
        extractROIFilter->SetInput( extractChannelFilter->GetOutput() );
        writer->SetInput( extractROIFilter->GetOutput() );
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


  return EXIT_SUCCESS;
}


