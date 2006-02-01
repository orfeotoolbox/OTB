// $Id$
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbGDALImageIO.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbGDALImageIOTestCanRead(int argc, char* argv[])
{
  try
  {
  	otb::GDALImageIO::Pointer lGDALImageIO = otb::GDALImageIO::New();
        bool lCanRead = lGDALImageIO->CanReadFile(argv[1]);
        if ( lCanRead == false)
        {
                std::cerr << "Erreur otb::GDALImageIO : impossible d'ouvrir l'image "<<argv[1]<<"."<<std::endl;
                return EXIT_FAILURE;
        }
  }
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Exception OTB attrappee dans exception ITK !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... )
  {
    std::cerr << "Exception OTB non attrappee !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
