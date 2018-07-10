/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbCurlHelperStub.h"

#include <iostream>
#include <fstream>

namespace otb
{

bool CurlHelperStub::TestUrlAvailability(const std::string& itkNotUsed(url)) const
{
  itkExceptionMacro(<< "otbCurlHelperStub: method undefined");
  return -1;
}

int CurlHelperStub::RetrieveUrlInMemory(const std::string& url, std::string& output) const
{
  if (url.compare("http://api.geonames.org/findNearbyPlaceName?lat=1.29&lng=103.78&username=otbteam") == 0)
    {
    output = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
        "<geonames>\n"
        "<geoname>\n"
        "<toponymName>Kent Ridge</toponymName>\n"
        "<name>Kent Ridge</name>\n"
        "<lat>1.29306</lat>\n"
        "<lng>103.78444</lng>\n"
        "<geonameId>1880515</geonameId>\n"
        "<countryCode>SG</countryCode>\n"
        "<countryName>Singapore</countryName>\n"
        "<fcl>P</fcl>\n"
        "<fcode>PPLX</fcode>\n"
        "<distance>0.59959</distance>\n"
        "</geoname>\n"
        "</geonames>\n";
    return 0;
    }
  itkExceptionMacro(<< "otbCurlHelperStub: value not recognized by stub");
  return -1;
}

int CurlHelperStub::RetrieveFile(const std::ostringstream& urlStream, std::string filename) const
{
  if (urlStream.str().compare("http://api.geonames.org/findNearbyPlaceName?lat=1.29&lng=103.78&username=otbteam") == 0)
    {
    std::ofstream file;
    file.open(filename);
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
      "<geonames>\n"
      "<geoname>\n"
      "<toponymName>Kent Ridge</toponymName>\n"
      "<name>Kent Ridge</name>\n"
      "<lat>1.29306</lat>\n"
      "<lng>103.78444</lng>\n"
      "<geonameId>1880515</geonameId>\n"
      "<countryCode>SG</countryCode>\n"
      "<countryName>Singapore</countryName>\n"
      "<fcl>P</fcl>\n"
      "<fcode>PPLX</fcode>\n"
      "<distance>0.59959</distance>\n"
      "</geoname>\n"
      "</geonames>\n";
    file.close();
    return 0;
    }
  itkExceptionMacro(<< "otbCurlHelperStub: value not recognized by stub");
  return -1;
}

int CurlHelperStub::RetrieveFile(const std::string& itkNotUsed(urlString), std::string itkNotUsed(filename)) const
{
  itkExceptionMacro(<< "otbCurlHelperStub: method undefined");
  return -1;
}

int CurlHelperStub::RetrieveFileMulti(const std::vector<std::string>& itkNotUsed(listURLs),
                                      const std::vector<std::string>& itkNotUsed(listFiles), int itkNotUsed(maxConnect)) const
{
  itkExceptionMacro(<< "otbCurlHelperStub: method undefined");
  return -1;
}

}
