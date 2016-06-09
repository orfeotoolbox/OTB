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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperApplicationRegistry.h"


int otbApplicationMemoryConnectTest(int argc, char * argv[])
{
  if(argc<4)
    {
    std::cerr<<"Usage: "<<argv[0]<<" application_path infname outfname"<<std::endl;
    return EXIT_FAILURE;
    }

  std::string path = argv[1];
  std::string infname = argv[2];
  std::string outfname = argv[3];

  otb::Wrapper::ApplicationRegistry::SetApplicationPath(path);
  
  otb::Wrapper::Application::Pointer app1 = otb::Wrapper::ApplicationRegistry::CreateApplication("Smoothing");

  otb::Wrapper::Application::Pointer app2 = otb::Wrapper::ApplicationRegistry::CreateApplication("Smoothing");

  if(app1.IsNull() || app2.IsNull())
    {
    std::cerr<<"Failed to create applications"<<std::endl;
    return EXIT_FAILURE;
    }

  app1->SetParameterString("in",infname);
  app1->Execute();
  
  app2->SetParameterString("out",outfname);

  // Connect app1 to app2
  app2->SetParameterInputImage("in",app1->GetParameterOutputImage("out"));

  app2->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
