/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   21 avril 2006
  Version   :   
  Role      :   Test l'extraction d'une ROI dans une image mono ou multi canal, dont les valeurs sont codées en "unsigned char"
                Les parametres de la ROI ne sont pas obligatoire, tout comme les canaux. Dans ce cas, les valauers par défaut 
                de la classe sont utilisées
                
  $Id$

=========================================================================*/

#include "itkExceptionObject.h"
#include "otbMultiChannelExtractROI.h"


int otbMultiChannelExtractROINew ( int argc, char ** argv )
{
  try 
    { 
//        typedef unsigned char  	                                InputPixelType;
//        typedef unsigned char  	                                OutputPixelType;

        typedef std::complex<float>  	                                InputPixelType;
        typedef std::complex<double>  	                                OutputPixelType;

        typedef otb::MultiChannelExtractROI< InputPixelType, 
                                             OutputPixelType >  ExtractROIFilterType;

        
	ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
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

