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

#include <iostream>
#include <fstream>

#include "otb_tinyxml.h"
#include "otbCurlHelper.h"

int otbIsNightlyRevision(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << argv[0] << " revision_number http_address_of_nightlynumber" << std::endl;
    return EXIT_FAILURE;
  }

  std::string wcRevision         = argv[1];
  std::string nightlyRevisionUrl = argv[2];

  otb::CurlHelper::Pointer curl = otb::CurlHelper::New();

  std::string curlOutput;
  curl->RetrieveUrlInMemory(nightlyRevisionUrl, curlOutput);

  std::string   nightlyRevision;
  TiXmlDocument doc;
  doc.Parse(curlOutput.c_str());

  TiXmlHandle docHandle(&doc);

  TiXmlElement* commitChild = docHandle.FirstChild("html").FirstChild("body").Child("div", 3).FirstChild("table").Child("tr", 4).Child("td", 1).Element();
  if (commitChild)
  {
    if (strcmp(commitChild->Attribute("class"), "sha1") == 0)
    {
      nightlyRevision = std::string(commitChild->GetText());
    }
  }

  if (nightlyRevision == wcRevision)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    std::cerr << "Nightly revision is " << nightlyRevision << " but working copy revision is " << wcRevision << std::endl;
    return EXIT_FAILURE;
  }
}
