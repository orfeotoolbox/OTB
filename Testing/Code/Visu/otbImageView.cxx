/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   Test les differents Image View
  Role      :   
  $Id$

=========================================================================*/
// OTB-FA-00026-CS
#include <stdlib.h>
#include "itkExceptionObject.h"

#include "otbImageViewer.h"
//#include "otbVectorImageView.h"
#include "otbGLVectorImageViewBase.h"
#include "otbPrincipalImageView.h"
#include "otbZoomImageView.h"
#include "otbScrollImageView.h"

int otbImageView( int argc, char ** argv )
{
  try 
    { 

        typedef otb::GLVectorImageViewBase<unsigned char>               GLVectorImageViewBaseType;
        typedef otb::PrincipalImageView<unsigned char>                  PrincipalImageViewType;
        typedef otb::VectorImageView<unsigned char>                     VectorImageViewType;
        typedef otb::ZoomImageView<unsigned char>                       ZoomImageViewType;
        typedef otb::ScrollImageView<unsigned char>                     ScrollImageViewType;

        VectorImageViewType::Pointer lVectorImageView = VectorImageViewType::New();
        GLVectorImageViewBaseType::Pointer lGLVectorImageViewBase = GLVectorImageViewBaseType::New();
        PrincipalImageViewType::Pointer lPrincipalImageView = PrincipalImageViewType::New();
        ZoomImageViewType::Pointer lZoomImageView = ZoomImageViewType::New();
        ScrollImageViewType::Pointer lScrollImageView = ScrollImageViewType::New();
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


