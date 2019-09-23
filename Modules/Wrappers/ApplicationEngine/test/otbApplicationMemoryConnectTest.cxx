/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(_MSC_VER)
#pragma warning(disable : 4786)
#endif

#include "otbWrapperApplicationRegistry.h"


int otbApplicationMemoryConnectTest(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Usage: " << argv[0] << " application_path infname outfname" << std::endl;
    return EXIT_FAILURE;
  }

  std::string path     = argv[1];
  std::string infname  = argv[2];
  std::string outfname = argv[3];

  otb::Wrapper::ApplicationRegistry::SetApplicationPath(path);

  otb::Wrapper::Application::Pointer app1 = otb::Wrapper::ApplicationRegistry::CreateApplication("Smoothing");

  otb::Wrapper::Application::Pointer app2 = otb::Wrapper::ApplicationRegistry::CreateApplication("Smoothing");

  otb::Wrapper::Application::Pointer app3 = otb::Wrapper::ApplicationRegistry::CreateApplication("Smoothing");

  otb::Wrapper::Application::Pointer app4 = otb::Wrapper::ApplicationRegistry::CreateApplication("ConcatenateImages");

  if (app1.IsNull() || app2.IsNull() || app3.IsNull() || app4.IsNull())
  {
    std::cerr << "Failed to create applications" << std::endl;
    return EXIT_FAILURE;
  }

  app1->SetParameterString("in", infname);
  app1->Execute();

  app2->SetParameterString("out", outfname);

  // Connect app1 to app2
  app2->SetParameterInputImage("in", app1->GetParameterOutputImage("out"));
  app2->Execute();

  app3->SetParameterString("in", infname);
  app3->Execute();

  app4->AddImageToParameterInputImageList("il", app2->GetParameterOutputImage("out"));
  app4->AddImageToParameterInputImageList("il", app3->GetParameterOutputImage("out"));
  app4->AddParameterStringList("il", infname);

  app4->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
