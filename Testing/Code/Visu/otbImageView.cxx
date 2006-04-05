/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#include <stdlib.h>
#include "itkExceptionObject.h"

//#include "otbImageView.h"
//#include "otbGLImageView.h"
//#include "otbGLColorImageView.h"
//#include "otbGLVectorImageViewBase.h"
//#include "otbGLVectorImageView.h"
//#include "otbPrincipalImageView.h"
//#include "otbZoomImageView.h"
#include "otbScrollImageView.h"

#include "itkImage.h"
#include "itkRGBPixel.h"


int main( int argc, char ** argv )
{
  try 
    { 

//        typedef otb::ImageView<unsigned char>    ViewType;
//        typedef otb::GLImageView<unsigned char,double>    ViewType;
//        typedef otb::VectorImageView<unsigned char>    ViewType;
//        typedef otb::GLVectorImageViewBase<unsigned char,double>    ViewType;
//        typedef otb::GLVectorImageView<unsigned char,double>    ViewType;
//        typedef otb::PrincipalImageView<unsigned char,double>    ViewType;
//        typedef otb::ZoomImageView<unsigned char,double>    ViewType;
        typedef otb::ScrollImageView<unsigned char,double>    ViewType;
        ViewType::Pointer lView = ViewType::New();
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


