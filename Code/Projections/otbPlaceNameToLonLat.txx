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
#ifndef _otbPlaceNameToLonLat_txx
#define _otbPlaceNameToLonLat_txx

#include "otbPlaceNameToLonLat.h"
#include "tinyxml.h"
#include <curl/curl.h>


namespace otb
{

/**
   * Constructor
   */

PlaceNameToLonLat::PlaceNameToLonLat()
{
  m_Lon = -1000.0;
  m_Lat = -1000.0;
  m_PlaceName = "Where everything started";
}

/**
   *
   */

void
PlaceNameToLonLat
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " m_Lon "  << m_Lon << std::endl;
  os << indent << " m_Lat "  << m_Lat << std::endl;
  os << indent << " m_PlaceName "  << m_PlaceName << std::endl;
}


bool PlaceNameToLonLat::Evaluate()
{
  string::size_type loc = m_PlaceName.find(" ", 0 );
  while (loc != string::npos)
  {
    m_PlaceName.replace(loc, 1, "+");
    loc = m_PlaceName.find(" ", loc);
  }

  if ((m_Lat == -1000.0) && (m_Lon == -1000.0))
  {
    std::ostringstream urlStream;
    urlStream << "http://maps.google.com/maps?q=";
    urlStream << m_PlaceName; 
    urlStream << "&sll=38.9594,-95.2655&sspn=119.526,360&output=kml&ie=utf-8&v=2.2&cv=4.2.0180.1134&hl=en";
    RetrieveXML(urlStream);
    ParseXMLGoogle();
  }
  
  if ((m_Lat == -1000.0) && (m_Lon == -1000.0))
  {
    std::ostringstream urlStream;
    urlStream << "http://maps.google.fr/maps?q=";
    urlStream << m_PlaceName; 
    urlStream << "&sll=38.9594,-95.2655&sspn=119.526,360&output=kml&ie=utf-8&v=2.2&cv=4.2.0180.1134&hl=en";
    RetrieveXML(urlStream);
    ParseXMLGoogle();
  }
  
  if ((m_Lat == -1000.0) && (m_Lon == -1000.0))
  {
    std::ostringstream urlStream;
    urlStream << "http://api.local.yahoo.com/MapsService/V1/geocode?appid=com.sun.blueprints.ui.geocoder&location=";
    urlStream << m_PlaceName; 
    RetrieveXML(urlStream);
    ParseXMLYahoo();
  }

  if ((m_Lat == -1000.0) && (m_Lon == -1000.0))
    {
    std::cout << "Search Error: fallback on the origin" << std::endl;
    m_Lat = 43.560204;
    m_Lon = 1.480736;
    return false;
    }
  return true;
}

void PlaceNameToLonLat::RetrieveXML(std::ostringstream& urlStream)
{
  
  CURL *curl;
  CURLcode res;

  FILE* output_file = fopen("out.xml","w");
  curl = curl_easy_init();

  std::cout << "URL data " << urlStream.str().data() << std::endl;


  char url[256];
  strcpy(url,urlStream.str().data());

//   std::cout << url << std::endl;
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);
    res = curl_easy_perform(curl);

    fclose(output_file);
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  
}


void PlaceNameToLonLat::ParseXMLYahoo()
{
  TiXmlDocument doc( "out.xml" );
  doc.LoadFile();
  TiXmlHandle docHandle( &doc );
  
  TiXmlElement* childLat = docHandle.FirstChild( "ResultSet" ).FirstChild( "Result" ).FirstChild( "Latitude" ).Element();
  if ( childLat )
  {
    m_Lat=atof(childLat->GetText());
  }
  TiXmlElement* childLon = docHandle.FirstChild( "ResultSet" ).FirstChild( "Result" ).FirstChild( "Longitude" ).Element();
  if ( childLon )
  {
    m_Lon=atof(childLon->GetText());
  }
  
}

void PlaceNameToLonLat::ParseXMLGoogle()
{
  TiXmlDocument doc( "out.xml" );
  doc.LoadFile();
  TiXmlHandle docHandle( &doc );
  
  TiXmlElement* childLat = docHandle.FirstChild( "kml" ).FirstChild( "Placemark" ).FirstChild( "LookAt" ).FirstChild( "latitude" ).Element();
  if ( childLat )
  {
    m_Lat=atof(childLat->GetText());
  }
  TiXmlElement* childLon = docHandle.FirstChild( "kml" ).FirstChild( "Placemark" ).FirstChild( "LookAt" ).FirstChild( "longitude" ).Element();
  if ( childLon )
  {
    m_Lon=atof(childLon->GetText());
  }
  
}

} // namespace otb

#endif
