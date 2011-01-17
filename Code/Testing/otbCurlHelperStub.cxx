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

int CurlHelperStub::TestUrlAvailability(const std::string& url) const
{}

int CurlHelperStub::RetrieveFile(const std::ostringstream& urlStream, std::string filename) const
{
  if (urlStream.str().compare("http://ws.geonames.org/findNearbyPlaceName?lat=1.29&lng=103.78") == 0)
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
    }
}

int CurlHelperStub::RetrieveFile(const std::string& urlString, std::string filename) const
{}

int CurlHelperStub::RetrieveFileMulti(const std::vector<std::string>& listURLs,
                      const std::vector<std::string>& listFiles,
                      int maxConnect) const
{}

}
