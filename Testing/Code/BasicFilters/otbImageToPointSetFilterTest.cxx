/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   23 janvier 2006
  Role      :  ImageToPointSetFilter class testing
  $Id$ 

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImageToPointSetFilter.h"
#include "itkImage.h"
#include "itkPointSet.h"

int otbImageToPointSetFilterTest(int , char *[] )
{


  const unsigned int Dimension = 2;

  typedef unsigned char  PixelType;

  typedef itk::Image< PixelType, Dimension  >           ImageType;
  typedef itk::PointSet<PixelType, Dimension>           PointSetType;
  typedef otb::ImageToPointSetFilter<ImageType,PointSetType> FunctionType;

  try
    {
//    FunctionType::Pointer function = FunctionType::New();

    } 
  catch( itk::ExceptionObject & exp )
    {
    std::cerr << "Exception thrown during the excecution of the generator " << std::endl;
    std::cerr << exp << std::endl;
    return EXIT_FAILURE;
    }
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}



