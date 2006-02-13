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
#include "otbMultiChannelExtractROI.h"


#include "itkImage.h"



template < typename  InputPixelType /*= unsigned char */, typename OutputPixelType /*= unsigned char*/ >
int generic_otbMultiChannelExtractROI( int argc, char ** argv, const char * inputFilename,const char * outputFilename)
{
  try 
    { 
//        const char * inputFilename  = argv[1];
//        const char * outputFilename = argv[2];


        typedef itk::Image<short,2> TypeImage;
        
        TypeImage::Pointer lima =  TypeImage::New();
        
        
/*        typedef _InputPixelType  	                                InputPixelType;
        typedef _OutputPixelType  	                                OutputPixelType;
*/
        typedef otb::MultiChannelExtractROI< InputPixelType, 
                                             OutputPixelType >  ExtractROIFilterType;

        typename ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
        int cpt(0),nbcanaux(0);

	while ( argv[cpt] != NULL )
	{
		std::string strArgv(argv[cpt]);
		if ( strArgv == "-startX" ) 		{ extractROIFilter->SetStartX((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetStartX("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-startY" ) 	{ extractROIFilter->SetStartY((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetStartY("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-sizeX" ) 	{ extractROIFilter->SetSizeX((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetSizeX("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-sizeY" ) 	{ extractROIFilter->SetSizeY((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetSizeY("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-channels" )
		{ 
			cpt++;
			bool searchChannels(true);
			while (searchChannels==true)
			{
				if ( argv[cpt] == NULL ) { searchChannels = false;}
				else
				//Regarde si ce n'est pas une autre otption
				{
					if ( argv[cpt][0] == '-' ) { searchChannels = false;}
					else
					{
						extractROIFilter->SetChannel((unsigned int)::atoi(argv[cpt]) );
                                                std::cout <<" ->SetChannel("<<::atoi(argv[cpt])<<")"<<std::endl;
						cpt++;nbcanaux++;
					}
				}
			}
		}
	}

	// Resume de la ligne de commande
	std::cout << " ROI selectionnee : startX "<<extractROIFilter->GetStartX()<<std::endl;
	std::cout << "                    startY "<<extractROIFilter->GetStartY()<<std::endl;
	std::cout << "                    sizeX  "<<extractROIFilter->GetSizeX()<<std::endl;
	std::cout << "                    sizeY  "<<extractROIFilter->GetSizeY()<<std::endl;
	std::cout << " Canaux selectionnes ("<<extractROIFilter->GetNbChannels()<<") : ";
	typename ExtractROIFilterType::ChannelsType lChannels;
	lChannels = extractROIFilter->GetChannels();
	for(int i=0 ; i< lChannels.size(); i++)
	{
		std::cout << lChannels[i]<<" ";
	}

        std::cout << " Extraction de "<<nbcanaux<<" de l'image source."<<std::endl;

        typedef otb::ImageFileReader< typename ExtractROIFilterType::InputImageType, itk::DefaultConvertPixelTraits< InputPixelType >  >       ReaderType;
//        typedef otb::ImageFileReader< typename ExtractROIFilterType::InputImageType, 
//                                        itk::DefaultConvertPixelTraits< typename _InputPixelType/*ExtractROIFilterType::InputImagePixelType*/ >  >       ReaderType;
        typedef otb::ImageFileWriter< typename ExtractROIFilterType::OutputImageType >           WriterType;
        typename ReaderType::Pointer reader = ReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();

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


int otbMultiChannelExtractROI ( int argc, char ** argv )
{
        std::string linputPixelType;
        std::string loutputPixelType;
        const char * inputFilename;
        const char * outputFilename;
        int cpt(1);
        //Si le format n'est pas spécifé, alors celui par defaut
        if( argv[cpt][0] == '-' )
        {
                linputPixelType = std::string(argv[cpt]);cpt++;
                inputFilename  = argv[cpt];cpt++;
        }
        else
        {
                linputPixelType = std::string("-uchar");
                inputFilename  = argv[cpt];cpt++;
        }
        if( argv[cpt][0] == '-' )
        {
                loutputPixelType = std::string(argv[cpt]);cpt++;
                outputFilename  = argv[cpt];cpt++;
        }
        else
        {
                loutputPixelType = std::string("-uchar");
                outputFilename  = argv[cpt];cpt++;
        }

        argc -= cpt;
        argv += cpt;
        std::cout << " -> "<<linputPixelType<<" pour "<<inputFilename<<std::endl;
        std::cout << " -> "<<loutputPixelType<<" pour "<<outputFilename<<std::endl;
        std::string pixelType;
        if (  (linputPixelType=="-uchar")&&(loutputPixelType=="-uchar") )               return (generic_otbMultiChannelExtractROI< unsigned char, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-ushort")&&(loutputPixelType=="-ushort") )        return (generic_otbMultiChannelExtractROI< unsigned short, unsigned short >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-uint")&&(loutputPixelType=="-uint") )            return (generic_otbMultiChannelExtractROI< unsigned int, unsigned int >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-float")&&(loutputPixelType=="-float") )          return (generic_otbMultiChannelExtractROI<float, float >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-double")&&(loutputPixelType=="-double") )        return (generic_otbMultiChannelExtractROI<double, double >( argc,argv,inputFilename,outputFilename) );
        // Type -> uchar
        else if (  (linputPixelType=="-ushort")&&(loutputPixelType=="-uchar") )         return (generic_otbMultiChannelExtractROI< unsigned short, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-uint")&&(loutputPixelType=="-uchar") )           return (generic_otbMultiChannelExtractROI< unsigned int, unsigned char >( argc,argv,inputFilename,outputFilename) );
//        else if (  (linputPixelType=="-float")&&(loutputPixelType=="-uchar") )          return (generic_otbMultiChannelExtractROI< float, unsigned char >( argc,argv,inputFilename,outputFilename) );
//        else if (  (linputPixelType=="-double")&&(loutputPixelType=="-uchar") )         return (generic_otbMultiChannelExtractROI< double, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else 
        {       
                std::cout << " Erreur : le format des images en entrée est mal précisé dans la ligne de commande !!!"<<std::endl;
                std::cout << "          valeurs autorisées : -uchar, -ushort, -uint, -float, -double"<<std::endl;
                std::cout << "          valeurs par defaut : -uchar"<<std::endl;
                return EXIT_FAILURE;
        }

return EXIT_FAILURE;
     
}        
        

  





