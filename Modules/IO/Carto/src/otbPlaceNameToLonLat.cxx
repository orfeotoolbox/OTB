/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbPlaceNameToLonLat.h"
#include "otb_tinyxml.h"
#include "otbCurlHelper.h"

namespace otb
{

PlaceNameToLonLat::PlaceNameToLonLat() : m_Lon(-1000.0), m_Lat(-1000.0), m_PlaceName("Where everything started"), m_RequestSucceed(false)
{
  m_Curl = CurlHelper::New();
}

void PlaceNameToLonLat::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " m_Lon " << m_Lon << std::endl;
  os << indent << " m_Lat " << m_Lat << std::endl;
  os << indent << " m_PlaceName " << m_PlaceName << std::endl;
}

bool PlaceNameToLonLat::Evaluate()
{
  std::string::size_type loc = m_PlaceName.find(" ", 0);
  while (loc != std::string::npos)
  {
    m_PlaceName.replace(loc, 1, "+");
    loc = m_PlaceName.find(" ", loc);
  }
  // TODO : replace unsafe ASCII character with "%" followed by their hex equivalent

  // Geonames service
  if ((m_Lat == -1000.0) && (m_Lon == -1000.0))
  {
    std::ostringstream urlStream;
    urlStream << "http://api.geonames.org/search?q=";
    urlStream << m_PlaceName;
    urlStream << "&maxRows=1&type=xml&orderby=relevance&username=otbteam";
    RetrieveXML(urlStream);
    if (m_RequestSucceed)
      ParseXMLGeonames();
  }

  // Google Maps service
  // if ((m_Lat == -1000.0) && (m_Lon == -1000.0))
  //   {
  //   std::ostringstream urlStream;
  //   urlStream << "http://maps.google.com/maps?q=";
  //   urlStream << m_PlaceName;
  //   urlStream << "&sll=38.9594,-95.2655&sspn=119.526,360&output=kml&ie=utf-8&v=2.2&cv=4.2.0180.1134&hl=en";
  //   RetrieveXML(urlStream);
  //   if ( m_RequestSucceed )
  //     ParseXMLGoogle();
  //   }

  // Yahoo new PlaceFinder API need an application ID to be able
  // to use it
  // if ((m_Lat == -1000.0) && (m_Lon == -1000.0))
  //   {
  //   std::ostringstream urlStream;
  //   urlStream << "http://api.local.yahoo.com/MapsService/V1/geocode?appid=com.sun.blueprints.ui.geocoder&location=";
  //   urlStream << m_PlaceName;
  //   RetrieveXML(urlStream);
  //   if ( m_RequestSucceed )
  //     ParseXMLYahoo();
  //   }

  if ((m_Lat == -1000.0) && (m_Lon == -1000.0))
  {
    std::cout << "Search Error: fallback on the origin" << std::endl;
    m_Lat = 43.560204;
    m_Lon = 1.480736;
    return false;
  }
  return true;
}

void PlaceNameToLonLat::RetrieveXML(const std::ostringstream& urlStream)
{
  try
  {
    m_RequestSucceed = true;
    m_Curl->RetrieveUrlInMemory(urlStream.str(), m_CurlOutput);
  }
  catch (itk::ExceptionObject&)
  {
    m_RequestSucceed = false;
  }
}

void PlaceNameToLonLat::ParseXMLYahoo()
{
  TiXmlDocument doc;
  doc.Parse(m_CurlOutput.c_str());

  TiXmlHandle   docHandle(&doc);
  TiXmlElement* childLat = docHandle.FirstChild("ResultSet").FirstChild("Result").FirstChild("Latitude").Element();
  if (childLat)
  {
    m_Lat = atof(childLat->GetText());
  }
  TiXmlElement* childLon = docHandle.FirstChild("ResultSet").FirstChild("Result").FirstChild("Longitude").Element();
  if (childLon)
  {
    m_Lon = atof(childLon->GetText());
  }
}

void PlaceNameToLonLat::ParseXMLGoogle()
{
  TiXmlDocument doc;
  doc.Parse(m_CurlOutput.c_str());

  TiXmlHandle   docHandle(&doc);
  TiXmlElement* childLat = docHandle.FirstChild("kml").FirstChild("Placemark").FirstChild("LookAt").FirstChild("latitude").Element();
  if (childLat)
  {
    m_Lat = atof(childLat->GetText());
  }
  TiXmlElement* childLon = docHandle.FirstChild("kml").FirstChild("Placemark").FirstChild("LookAt").FirstChild("longitude").Element();
  if (childLon)
  {
    m_Lon = atof(childLon->GetText());
  }
}

void PlaceNameToLonLat::ParseXMLGeonames()
{
  TiXmlDocument doc;
  doc.Parse(m_CurlOutput.c_str());

  TiXmlHandle   docHandle(&doc);
  TiXmlElement* childLat = docHandle.FirstChild("geonames").FirstChild("geoname").FirstChild("lat").Element();
  if (childLat)
  {
    m_Lat = atof(childLat->GetText());
  }
  TiXmlElement* childLon = docHandle.FirstChild("geonames").FirstChild("geoname").FirstChild("lng").Element();
  if (childLon)
  {
    m_Lon = atof(childLon->GetText());
  }
}

} // namespace otb
