// $Id$
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbCAIImageIO.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbCAIImageIOTestCanWrite(int argc, char* argv[])
{
  try
  {
  	otb::CAIImageIO::Pointer lCAIImageIO = otb::CAIImageIO::New();
        bool lCanRead = lCAIImageIO->CanWriteFile(argv[1]);
        if ( lCanRead == false)
        {
                std::cerr << "Erreur otb::CAIImageIO : impossible de creer l'image "<<argv[1]<<"."<<std::endl;
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
