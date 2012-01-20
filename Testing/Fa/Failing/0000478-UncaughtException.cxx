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

#include "otbWrapperApplicationRegistry.h"

int main(int argc,char* argv[])
{
  typedef otb::Wrapper::Application AppType;
  
  if (argc != 4)
    {
    std::cout << "Usage: 0000478-UncaughtException stereo_ref stereo_sec output_file" << std::endl;
    return 1;
    }
  
  otb::Wrapper::ApplicationRegistry::SetApplicationPath("/home2/gpasero/ORFEO-TOOLBOX/otb-build/OTB-bin/bin");
  AppType::Pointer myApp = otb::Wrapper::ApplicationRegistry::CreateApplication("StereoSensorModelToElevationMap");
  
  if (myApp.IsNull())
    {
    std::cout << "Application not created" << std::endl;
    return 1;
    }
  
  myApp->Init();
  
  myApp->SetParameterString("ref", argv[1]);
  myApp->SetParameterString("sec", argv[2]);
  myApp->SetParameterString("out", argv[3]);
  myApp->SetParameterString("elev","dem");
  myApp->SetParameterString("elev.dem.path", "FAKE_DEM_PATH");
  myApp->SetParameterString("elev.dem.geoid", "FAKE_GEOID_PATH");
  
  std::cout << "Try-catch section" << std::endl;
  try
    {
    myApp->ExecuteAndWriteOutput();
    }
  catch(...)
    {
    std::cerr << "An unknown exception has been caught !" << std::endl;
    }
  
  return 0;
}
