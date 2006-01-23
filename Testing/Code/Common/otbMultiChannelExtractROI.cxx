/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test l'extraction d'une ROI dans une image multi canal, dont les valeurs sont codées en "unsigned char"
  $Id$

=========================================================================*/

#include "itkExceptionObject.h"


#include "otbMultiChannelExtractROI.h"
#include "itkVectorImage.h"

#include "itkVectorContainer.h"
#include "itkImageRegionIteratorWithIndex.h"

int otbMultiChannelExtractROItoto( int argc, char ** argv )
{
#if 0

  try 
    { 
        unsigned int  startX(1);
        unsigned int  startY(2);
        unsigned int  sizeX(2);
        unsigned int  sizeY(1);
        unsigned int  nbCanaux(3);

        typedef unsigned short 	                                InputPixelType;
        typedef unsigned short 	                                OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::VectorImage< InputPixelType,  Dimension >        InputImageType;
        typedef itk::VectorImage< OutputPixelType, Dimension >        OutputImageType;
        InputImageType::Pointer lInputImage = InputImageType::New();
        OutputImageType::Pointer lOutputImage = OutputImageType::New();

	typedef itk::ImageRegionIteratorWithIndex< InputImageType > 	IteratorType;

  	InputImageType::RegionType                              inputRegion;
  	InputImageType::RegionType::IndexType                   inputStart;
  	InputImageType::RegionType::SizeType                    size;
  	InputImageType::SpacingType                             spacing;
//  	InputImageType::PointType                               inputOrigin;
  	double   inputOrigin[ 2 ];

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
  	lInputImage->SetRegions( inputRegion );
  	lInputImage->SetSpacing( spacing );
  	lInputImage->SetOrigin(  inputOrigin );
  	lInputImage->Allocate();
	IteratorType      		inputIt(  lInputImage,   inputRegion );
        InputImageType::IndexType 			indexPixel;

        for ( int x=0 ; x < size[1] ; x++ )
        {
        
                for ( int y=0 ; y < size[0] ; y++ )
                {
                
                        InputImageType::PixelType lPixel;
                        InputPixelType lCanalValue;
                        indexPixel[0] = x;
                        indexPixel[1] = y;
                        lCanalValue = x*y;       lPixel.SetElement((unsigned long)(0),lCanalValue);
                        lCanalValue = x*y*10;    lPixel.SetElement((unsigned long)(1),lCanalValue);
                        lCanalValue = x*y*100;   lPixel.SetElement((unsigned long)(2),lCanalValue);
//                        InputImageType::PixelType::STLContainerType Vector;
                        
//                        InputImageType::PixelType lPixel2(static_cast<InputImageType::PixelType>(*lPixel));
//                        inputIt.Set(*lPixel);
//                        ++inputIt;
                        lInputImage->SetPixel(indexPixel, lPixel);
                }
        }

/*  	for ( outputIt.GoToBegin() ; !outputIt.IsAtEnd(); ++outputIt)
    	{
		//Random Value
    		outputIt.Set( (PixelType)(facteur*(TFacteur)generator->GetVariate()) );
	}
*/


        typedef otb::MultiChannelExtractROI<    InputPixelType, 
                                                OutputPixelType >   FilterType;
/*        FilterType::Pointer filter = FilterType::New();
        
        filter->SetStartX( startX );
        filter->SetStartY( startY );
        filter->SetSizeX( sizeX );
        filter->SetSizeY( sizeY );
        
//        filter->SetChannel( 1 );
*/
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

#endif

  return EXIT_SUCCESS;
}

