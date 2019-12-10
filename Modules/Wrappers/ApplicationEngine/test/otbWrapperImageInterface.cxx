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


int otbWrapperImageInterface(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Usage: " << argv[0] << " application_path  input_image  output_txt" << std::endl;
    return EXIT_FAILURE;
  }

  std::string path   = argv[1];
  std::string input  = argv[2];
  std::string output = argv[3];

  otb::Wrapper::ApplicationRegistry::SetApplicationPath(path);

  otb::Wrapper::Application::Pointer app1 = otb::Wrapper::ApplicationRegistry::CreateApplication("Smoothing");

  if (app1.IsNull())
  {
    std::cerr << "Failed to create application" << std::endl;
    return EXIT_FAILURE;
  }

  std::ofstream ofs(output);
  if (!ofs.is_open())
  {
    fprintf(stderr, "Error, can't open file");
    return EXIT_FAILURE;
  }

  app1->SetParameterString("in", input);
  app1->Execute();

  ofs << "Size: " << app1->GetImageSize("out") << std::endl;
  ofs << "Origin: " << app1->GetImageOrigin("out") << std::endl;
  ofs << "Spacing: " << app1->GetImageSpacing("out") << std::endl;
  ofs << "Keywordlist: " << std::endl;
  otb::ImageKeywordlist kwl = app1->GetImageKeywordlist("out");
  kwl.Print(ofs);
  ofs << "ProjectionRef:" << std::endl;
  ofs << app1->GetImageProjection("out") << std::endl;

  itk::MetaDataDictionary dict = app1->GetImageMetaData("out");
  ofs << "Dictionary keys:" << std::endl;
  for (auto& key : dict.GetKeys())
  {
    ofs << "  - " << key << std::endl;
  }

  otb::Wrapper::ImageBaseType::RegionType region;
  region.SetIndex(0, 10);
  region.SetIndex(1, 10);
  region.SetSize(0, 7);
  region.SetSize(1, 7);

  ofs << "RAM usage (in Bytes): " << app1->PropagateRequestedRegion("out", region) << std::endl;
  ofs << "Input requested: " << app1->GetImageRequestedRegion("in") << std::endl;

  otb::Wrapper::Application::Pointer app2 = otb::Wrapper::ApplicationRegistry::CreateApplication("ConcatenateImages");
  app2->AddParameterStringList("il", input);
  app2->AddParameterStringList("il", input);
  app2->Execute();

  ofs << "Number of bands [il]: " << app2->GetImageNbBands("il", 0) << "+" << app2->GetImageNbBands("il", 1) << std::endl;
  ofs << "Number of bands [out]: " << app2->GetImageNbBands("out") << std::endl;
  ofs.close();
  return EXIT_SUCCESS;
}
