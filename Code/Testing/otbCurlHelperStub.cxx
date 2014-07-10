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

#include "otbCurlHelperStub.h"

#include <iostream>
#include <fstream>
#include "otbMacro.h"

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
    file.open(filename.c_str());
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
