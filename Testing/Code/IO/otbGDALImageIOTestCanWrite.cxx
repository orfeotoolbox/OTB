// $Id$
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbGDALImageIO.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbGDALImageIOTestCanWrite(int argc, char* argv[])
{
  try
  {
  	otb::GDALImageIO::Pointer lGDALImageIO = otb::GDALImageIO::New();
        bool lCanWrite = lGDALImageIO->CanWriteFile(argv[1]);
        if ( lCanWrite == false)
        {
                std::cerr << "Erreur otb::GDALImageIO : impossible d'ecrire l'image "<<argv[1]<<"."<<std::endl;
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
