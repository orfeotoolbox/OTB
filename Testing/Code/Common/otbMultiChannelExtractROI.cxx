/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test l'extraction d'une ROI dans une image multi canal, dont les valeurs sont codées en "unsigned char"
  $Id$

=========================================================================*/

#include <iostream>
#include <fstream>

#include "itkExceptionObject.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkVectorImage.h"
#include "itkVariableLengthVector.h"

#include "otbMultiChannelExtractROI.h"

//*******************************************************************************************************************
template <class PixelType, unsigned int VImageDimension>
void displayImage(	typename itk::Image< PixelType,VImageDimension >::Pointer pInImage, std::ostream & ressource )
{
//	displayImage<PixelType,VImageDimension>( static_cast<(itk::Image<PixelType,VImageDimension>::ConstPointer)> pInImage );
	typedef itk::Image< PixelType, VImageDimension >  		ImageType;
  	typedef itk::ImageRegionConstIteratorWithIndex< ImageType > 	ConstIteratorType;
  	typename ImageType::RegionType 					inputRegion;
  	typename ImageType::RegionType::SizeType 			size;

	ressource <<"*****  ITK::IMAGE  *****************************************"<<std::endl;
	ressource <<" Structure :"<<std::endl;
	ressource <<pInImage;
	size = pInImage->GetRequestedRegion().GetSize();
	ressource <<" Pixels : "<<size<<std::endl;
	
  	ConstIteratorType 		inputIt( pInImage , pInImage->GetRequestedRegion()  );
	PixelType 			lPixel;
	inputIt.GoToBegin();
	
	unsigned int nbPointsAffich=10,cptPointsX=0,cptPointsY=0;
	if(nbPointsAffich > size[0]) nbPointsAffich = size[0];
	if(nbPointsAffich > size[1]) nbPointsAffich = size[1];
	
  	for ( unsigned int i = 0 ;  (i < size[1]) && (!inputIt.IsAtEnd()) && (cptPointsY<nbPointsAffich) ; i++,cptPointsY++ )
    	{
		cptPointsX = 0;
	  	for ( unsigned int j = 0 ;  j < size[0] && !inputIt.IsAtEnd() ; j++,cptPointsX++)
    		{
			//Si valeur "char", etc... codée sur 1 octets, on cast en "int" pour afficher une valeur numérique (bourrin mais bon ;-)
			//Affiche que les nbPointsAffich de la ligne 
			if (cptPointsX<nbPointsAffich)
			{
				ressource << "   ";
				lPixel  = inputIt.Get();
				if( sizeof(PixelType) == 1 ) ressource << (int)lPixel;
				else ressource << lPixel;
			}
			++inputIt;
		}
    		ressource <<std::endl;
	}
    	ressource <<std::endl;
 
}

//*******************************************************************************************************************
template <class InputImageType>
void displayVectorImage2(	typename InputImageType::Pointer pInImage, std::ostream & ressource )
{
	typedef InputImageType  		ImageType;
  	typedef itk::ImageRegionIteratorWithIndex< ImageType > 	IteratorType;
  	typename ImageType::RegionType 					inputRegion;
  	typename ImageType::RegionType::SizeType 			size;
	typename ImageType::PixelType 			                lPixel;
	typename ImageType::InternalPixelType 			        lInternalPixel;
	ressource <<"*****  ITK::IMAGE  *****************************************"<<std::endl;
	ressource <<" Structure :"<<std::endl;
	ressource <<pInImage;
	size = pInImage->GetRequestedRegion().GetSize();
	ressource <<" Pixels : "<<size<<std::endl;
	
  	IteratorType 		inputIt( pInImage , pInImage->GetRequestedRegion()  );
	inputIt.GoToBegin();
	
	unsigned int nbPointsAffich=10,cptPointsX=0,cptPointsY=0;
	if(nbPointsAffich > size[0]) nbPointsAffich = size[0];
	if(nbPointsAffich > size[1]) nbPointsAffich = size[1];
	
  	for ( unsigned int i = 0 ;  (i < size[1]) && (!inputIt.IsAtEnd()) && (cptPointsY<nbPointsAffich) ; i++,cptPointsY++ )
    	{
		cptPointsX = 0;
	  	for ( unsigned int j = 0 ;  j < size[0] && !inputIt.IsAtEnd() ; j++,cptPointsX++)
    		{
			//Si valeur "char", etc... codée sur 1 octets, on cast en "int" pour afficher une valeur numérique (bourrin mais bon ;-)
			//Affiche que les nbPointsAffich de la ligne 
			if (cptPointsX<nbPointsAffich)
			{
				ressource << "   (";
				lPixel  = inputIt.Get();
                                unsigned int nbComponent = lPixel.GetNumberOfElements();
                                unsigned int lcomponent = 0;
                                for ( lcomponent = 0 ; lcomponent < (nbComponent  - 1) ; lcomponent++ )
                                {
                                        lInternalPixel = lPixel.GetElement(lcomponent);
				        if( sizeof(lInternalPixel) == 1 ) ressource << (int)lInternalPixel <<",";
				        else ressource << lInternalPixel <<",";
                                }
                                lInternalPixel = lPixel.GetElement(lcomponent);
				if( sizeof(lInternalPixel) == 1 ) ressource << (int)lInternalPixel <<")";
				else ressource << lInternalPixel <<")";
                                
			}
			++inputIt;
		}
    		ressource <<std::endl;
	}
    	ressource <<std::endl;
 
}
//*******************************************************************************************************************
template <class InputImageType>
void displayVectorImage(	typename InputImageType::Pointer pInImage, std::ostream & ressource, std::ostream & fileressource )
{
	typedef InputImageType  		ImageType;
  	typedef itk::ImageRegionIteratorWithIndex< ImageType > 	IteratorType;
	typename ImageType::PixelType 			                lPixel;
	typename ImageType::InternalPixelType 			        lInternalPixel;
	ressource <<"*****  ITK::IMAGE  *****************************************"<<std::endl;
	ressource <<" Structure :"<<std::endl;
	ressource <<pInImage;
	fileressource <<"*****  ITK::IMAGE  *****************************************"<<std::endl;
        fileressource	 << "Size image : "<<pInImage->GetRequestedRegion().GetSize()<<std::endl;
        
  	IteratorType 		inputIt( pInImage , pInImage->GetRequestedRegion()  );
	inputIt.GoToBegin();
	

  	while( !inputIt.IsAtEnd()) 
    	{
		ressource << "   (";
		fileressource << "   (";
		lPixel  = inputIt.Get();
                unsigned int nbComponent = lPixel.GetNumberOfElements();
                unsigned int lcomponent = 0;
                for ( lcomponent = 0 ; lcomponent < (nbComponent  - 1) ; lcomponent++ )
                {
                        lInternalPixel = lPixel.GetElement(lcomponent);
			if( sizeof(lInternalPixel) == 1 ) {ressource << (int)lInternalPixel <<","; fileressource << (int)lInternalPixel <<",";}
			else {ressource << lInternalPixel <<",";fileressource << lInternalPixel <<",";}
                }
                lInternalPixel = lPixel.GetElement(lcomponent);
		if( sizeof(lInternalPixel) == 1 ) {ressource << (int)lInternalPixel <<")";fileressource << (int)lInternalPixel <<")";}
		else {ressource << lInternalPixel <<")";fileressource << lInternalPixel <<")";}
                  
		++inputIt;
	}
    	ressource <<std::endl;
    	fileressource <<std::endl;
 
}


int otbMultiChannelExtractROI( int argc, char ** argv )
{

  try 
    { 
        const char * filename(argv[1]);
        unsigned int  startX(1);
        unsigned int  startY(2);
        unsigned int  sizeX(2);
        unsigned int  sizeY(1);
        unsigned int  nbCanaux(3);

        typedef unsigned short 	                                InputPixelType;
        typedef unsigned short 	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        std::cout << "  Instanciation otb::MultiChannelExtractROI ..."<<std::endl;
        typedef otb::MultiChannelExtractROI<    InputPixelType, 
                                                OutputPixelType >   FilterType;

        typedef FilterType::InputImageType  InputImageType;
        typedef FilterType::OutputImageType  OutputImageType;

        std::cout << "  Instanciation des images Input/Output ..."<<std::endl;
        InputImageType::Pointer lInputImage = InputImageType::New();
        OutputImageType::Pointer lOutputImage = OutputImageType::New();

	typedef itk::ImageRegionIteratorWithIndex< InputImageType > 	IteratorType;
	typedef itk::ImageRegionIteratorWithIndex< OutputImageType > 	OutputIteratorType;

  	OutputImageType::RegionType                             outputRegion;
  	InputImageType::RegionType                              inputRegion;
  	InputImageType::RegionType::IndexType                   inputStart;
  	InputImageType::RegionType::SizeType                    size;
  	InputImageType::SpacingType                             spacing;
  	InputImageType::OffsetValueType                         offset;
//  	InputImageType::PointType                               inputOrigin;
  	double   inputOrigin[ 2 ];
        
        typedef  InputImageType::PixelType  TInputPixelType;

        // Création de l'image de test
        spacing[0] = 1.0;
        spacing[1] = 1.0;
        inputOrigin[0] = 0.;
        inputOrigin[1] = 0.;
        
  	inputStart[0] = 0;
  	inputStart[1] = 0;
	size[0] = 4;
	size[1] = 6;
  	inputRegion.SetSize( size );
  	inputRegion.SetIndex( inputStart );
	//Modification des structures
        lInputImage->SetVectorLength(3);

  	lInputImage->SetRegions( inputRegion );
  	lInputImage->SetSpacing( spacing );
  	lInputImage->SetOrigin(  inputOrigin );
  	lInputImage->Allocate();
	IteratorType      		inputIt(  lInputImage,   inputRegion );
        InputImageType::IndexType 			indexPixel;


        std::cout << "  Boucle sur l'initialisation de l'image Input ..."<<std::endl;
        for ( int x=0 ; x < size[1] ; x++ )
        {
        
                for ( int y=0 ; y < size[0] ; y++ )
                {
                        indexPixel[0] = x;
                        indexPixel[1] = y;

                        InputImageType::PixelType lPixel;
                        InputImageType::InternalPixelType lCanalValue;
                        lPixel.Reserve(lInputImage->GetVectorLength());
                        lCanalValue = (x+1)*(y+1);       lPixel.SetElement((unsigned long)(0),lCanalValue);
                        lCanalValue = (x+1)*(y+1)*10;    lPixel.SetElement((unsigned long)(1),lCanalValue);
                        lCanalValue = (x+1)*(y+1)*100;   lPixel.SetElement((unsigned long)(2),lCanalValue);
                        lInputImage->SetPixel(indexPixel, lPixel);
                        
                }
        }
        std::ofstream file;
        file.open(filename);
        file << "Image d'entree :"<<std::endl;
        displayVectorImage<InputImageType>(	lInputImage, std::cout, file );

        std::cout << "  Initialisation du filtre d'Extraction ..."<<std::endl;
        FilterType::Pointer filter = FilterType::New();
        
        filter->SetStartX( startX );
        filter->SetStartY( startY );
        filter->SetSizeX( sizeX );
        filter->SetSizeY( sizeY );
        
        filter->SetInput( lInputImage );
        std::cout << "  Update ..."<<std::endl;
        filter->Update();
        
        
        file << "Image extraite :"<<std::endl;
        displayVectorImage<OutputImageType>(	filter->GetOutput(), std::cout, file );
        file.close();

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


