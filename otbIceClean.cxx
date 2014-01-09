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
#include "otbIceViewer.h"
#include "otbDEMHandler.h"



void error_callback(int error, const char* description)
{
  std::cerr<<description<<std::endl;
}

int main(int argc, char * argv[])
{
  if(argc < 2)
    {
    std::cerr<<"Usage: "<<argv[0]<<" img1 [img2 img3 img4 img5]"<<std::endl<<std::endl;
    
    return EXIT_FAILURE;
    }

  char * demdir = getenv("OTB_DEM_DIR");
  char * geoidfile = getenv("OTB_GEOID_FILE");

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();
  
  if(demdir != NULL)
    {
    std::cout<<"Configuring DEM directory: "<<demdir<<std::endl;
    demHandler->OpenDEMDirectory(demdir);
    }

  if(geoidfile != NULL)
    {
    std::cout<<"Configuring geoid file: "<<geoidfile<<std::endl;
    demHandler->OpenGeoidFile(geoidfile);
    }

  otb::IceViewer::Pointer viewer = otb::IceViewer::GetInstance();

  // Initialize viewer
  viewer->Initialize(800,600);

  for(unsigned int i = 1; i<argc;++i)
    {
    viewer->AddImage(argv[i],argv[i]);
    }

  viewer->Start();
  
  
  return EXIT_SUCCESS;
}
