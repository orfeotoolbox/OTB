/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test l'extraction d'une ROI dans une image mono ou multi canal, dont les valeurs sont codées en "unsigned char"
                Les parametres de la ROI ne sont pas obligatoire, tout comme les canaux. Dans ce cas, les valauers par défaut 
                de la classe sont utilisées
                
  $Id$

=========================================================================*/

#include "itkExceptionObject.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiToMonoChannelExtractROI.h"

int otbMultiToMonoChannelExtractROI ( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
        
        typedef unsigned char  	                                InputPixelType;
        typedef unsigned char  	                                OutputPixelType;

        typedef otb::MultiToMonoChannelExtractROI< InputPixelType, 
                                             OutputPixelType >  ExtractROIFilterType;

        ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
        int cpt(3),nbcanaux(0);

	while ( argv[cpt] != NULL )
	{
		std::string strArgv(argv[cpt]);
		if ( strArgv == "-startX" ) 		{ extractROIFilter->SetStartX((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetStartX("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-startY" ) 	{ extractROIFilter->SetStartY((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetStartY("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-sizeX" ) 	{ extractROIFilter->SetSizeX((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetSizeX("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-sizeY" ) 	{ extractROIFilter->SetSizeY((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetSizeY("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-channel" )       { extractROIFilter->SetChannel((unsigned int)::atoi(argv[cpt+1]));std::cout <<" ->SetChannel("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
	}

	// Resume de la ligne de commande
	std::cout << " ROI selectionnee : startX "<<extractROIFilter->GetStartX()<<std::endl;
	std::cout << "                    startY "<<extractROIFilter->GetStartY()<<std::endl;
	std::cout << "                    sizeX  "<<extractROIFilter->GetSizeX()<<std::endl;
	std::cout << "                    sizeY  "<<extractROIFilter->GetSizeY()<<std::endl;
	std::cout << " Canal selectionne : ("<<extractROIFilter->GetChannel()<<") : ";

        typedef otb::ImageFileReader< ExtractROIFilterType::InputImageType, itk::DefaultConvertPixelTraits< InputPixelType >  >       ReaderType;
        typedef otb::ImageFileWriter< ExtractROIFilterType::OutputImageType >           WriterType;
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        reader->Update(); //Necessaire pour connaitre le nombre de canaux dans l'image
        writer->SetFileName( outputFilename );
        extractROIFilter->SetInput( reader->GetOutput() );
        
        writer->SetInput( extractROIFilter->GetOutput() );
        writer->Update(); 
        std::cout << " Nb canaux dans l'image d'entree : "<< reader->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;
        std::cout << " Nb canaux dans l'image de sortie : "<<extractROIFilter->GetOutput()->GetNumberOfComponentsPerPixel() <<std::endl;
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




