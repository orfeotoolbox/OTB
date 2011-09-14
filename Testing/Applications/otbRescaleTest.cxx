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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationRegistry.h"

using otb::Wrapper::Application;
using otb::Wrapper::ApplicationRegistry;

int otbRescaleTest(int argc, char* argv[])
{
  ApplicationRegistry::SetApplicationPath(argv[1]);

  const char* in = argv[2];
  const char* out = argv[3];
  const float outmin = atof(argv[4]);
  const float outmax = atof(argv[5]);

  Application::Pointer app = ApplicationRegistry::CreateApplication("Rescale");

  app->SetParameterString("in", in );
  app->SetParameterString("out", out );
  app->SetParameterFloat("outmin", outmin);
  app->SetParameterFloat("outmax", outmax);
  app->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
