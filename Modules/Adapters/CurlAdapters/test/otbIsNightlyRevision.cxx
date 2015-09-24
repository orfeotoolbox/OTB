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
#include <iostream>
#include <fstream>

#include "otb_tinyxml.h"
#include "otbCurlHelper.h"

int otbIsNightlyRevision(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << argv[0] << " revision_number http_address_of_nightlynumber" << std::endl;
    return EXIT_FAILURE;
    }

  std::string wcRevision = argv[1];
  std::string nightlyRevisionUrl = argv[2];

  otb::CurlHelper::Pointer curl = otb::CurlHelper::New();

  std::string curlOutput;
  curl->RetrieveUrlInMemory(nightlyRevisionUrl, curlOutput);

  std::string nightlyRevision;
  TiXmlDocument doc;
  doc.Parse(curlOutput.c_str());

  TiXmlHandle docHandle(&doc);

  TiXmlElement* commitChild = docHandle.FirstChild("html").FirstChild("body").Child("div",3).FirstChild("table").Child("tr",4).Child("td",1).Element();
  if (commitChild)
    {
    if (strcmp(commitChild->Attribute("class"),"sha1") == 0)
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
