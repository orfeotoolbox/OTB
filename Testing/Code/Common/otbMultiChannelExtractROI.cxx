/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkExceptionObject.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"


#include "otbImage.h"



template < typename  InputPixelType /*= unsigned char */, typename OutputPixelType /*= unsigned char*/ >
int generic_otbMultiChannelExtractROI( int argc, char * argv[], const char * inputFilename,const char * outputFilename)
{
  typedef otb::MultiChannelExtractROI< InputPixelType,
    OutputPixelType >  ExtractROIFilterType;

  typename ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  int cpt(0),nbcanaux(0);

  while ( argv[cpt] != NULL )
    {
      std::string strArgv(argv[cpt]);
      if ( strArgv == "-startX" )     { extractROIFilter->SetStartX((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetStartX("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
      else if ( strArgv == "-startY" )   { extractROIFilter->SetStartY((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetStartY("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
      else if ( strArgv == "-sizeX" )   { extractROIFilter->SetSizeX((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetSizeX("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
      else if ( strArgv == "-sizeY" )   { extractROIFilter->SetSizeY((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetSizeY("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
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
  for(unsigned int i=0 ; i< lChannels.size(); i++)
    {
      std::cout << lChannels[i]<<" ";
    }

  std::cout << " Extraction de "<<nbcanaux<<" de l'image source."<<std::endl;

  // OTB-FA-00008-CS
  typedef otb::ImageFileReader< ITK_TYPENAME ExtractROIFilterType::InputImageType >       ReaderType;
  typedef otb::ImageFileWriter< ITK_TYPENAME ExtractROIFilterType::OutputImageType >           WriterType;
  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( inputFilename  );
  //THOMAS
  //        reader->Update(); //Necessaire pour connaitre le nombre de canaux dans l'image
  writer->SetFileName( outputFilename );
  extractROIFilter->SetInput( reader->GetOutput() );

  writer->SetInput( extractROIFilter->GetOutput() );
  writer->Update();
  std::cout << " Nb canaux dans l'image d'entree : "<< reader->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;
  std::cout << " Nb canaux dans l'image de sortie : "<<extractROIFilter->GetOutput()->GetNumberOfComponentsPerPixel() <<std::endl;



  return EXIT_SUCCESS;
}


int otbMultiChannelExtractROI ( int argc, char * argv[] )
{
        std::string linputPixelType;
        std::string loutputPixelType;
        const char * inputFilename;
        const char * outputFilename;
        int cpt(1);
        //Si le format n'est pas sp�cif�, alors celui par defaut
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
        else if (  (linputPixelType=="-char")&&(loutputPixelType=="-char") )        return (generic_otbMultiChannelExtractROI< char, char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-ushort")&&(loutputPixelType=="-ushort") )        return (generic_otbMultiChannelExtractROI< unsigned short, unsigned short >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-uint")&&(loutputPixelType=="-uint") )            return (generic_otbMultiChannelExtractROI< unsigned int, unsigned int >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-float")&&(loutputPixelType=="-float") )          return (generic_otbMultiChannelExtractROI<float, float >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-double")&&(loutputPixelType=="-double") )        return (generic_otbMultiChannelExtractROI<double, double >( argc,argv,inputFilename,outputFilename) );
        // Type -> uchar
        else if (  (linputPixelType=="-ushort")&&(loutputPixelType=="-uchar") )         return (generic_otbMultiChannelExtractROI< unsigned short, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-uint")&&(loutputPixelType=="-uchar") )           return (generic_otbMultiChannelExtractROI< unsigned int, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-float")&&(loutputPixelType=="-uchar") )          return (generic_otbMultiChannelExtractROI< float, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-double")&&(loutputPixelType=="-uchar") )         return (generic_otbMultiChannelExtractROI< double, unsigned char >( argc,argv,inputFilename,outputFilename) );
        // uchar -> Type
        else if (  (linputPixelType=="-uchar")&&(loutputPixelType=="-double") )         return (generic_otbMultiChannelExtractROI< unsigned char, double >( argc,argv,inputFilename,outputFilename) );
        else
        {
                std::cout << " Erreur : le format des images en entree est mal precise dans la ligne de commande !!!"<<std::endl;
                std::cout << "          valeurs autorisees : -uchar, -char, -ushort, -uint, -float, -double"<<std::endl;
                std::cout << "          valeurs par defaut : -uchar"<<std::endl;
                return EXIT_FAILURE;
        }

return EXIT_FAILURE;

}








