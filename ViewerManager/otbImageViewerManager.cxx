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
#include "otbImageViewerManager.h"

int main(int argc, char* argv[])
{
 try 
    {
  typedef double PixelType;
  typedef otb::ImageViewerManager<PixelType> ManagerType;
  ManagerType::Pointer manager = ManagerType::New();
  manager->Show();

  for(int i = 1; i<argc;++i)
    {
      manager->OpenImage(argv[i]);
      Fl::check();
    }
  return Fl::run();
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Following otbException catch :" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( std::bad_alloc & err ) 
    { 
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Unknown Exception found !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
