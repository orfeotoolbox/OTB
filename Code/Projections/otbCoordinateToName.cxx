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

#include "otbCoordinateToName.h"
#include "tinyxml.h"
#include <curl/curl.h>
#include "otbMacro.h"

namespace otb
{

/**
   * Constructor
   */

CoordinateToName::CoordinateToName()
{
  m_Lon = -1000.0;
  m_Lat = -1000.0;
  m_PlaceName = "";
  m_CountryName = "";
  m_TempFileName = "out-SignayriUt1.xml";
}

/**
   *
   */

void
CoordinateToName
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " m_Lon "  << m_Lon << std::endl;
  os << indent << " m_Lat "  << m_Lat << std::endl;
  os << indent << " m_PlaceName "  << m_PlaceName << std::endl;
}


bool CoordinateToName::Evaluate()
{

  std::ostringstream urlStream;
  urlStream << "http://ws.geonames.org/findNearbyPlaceName?lat=";
  urlStream << m_Lat;
  urlStream << "&lng=";
  urlStream << m_Lon;
  otbMsgDevMacro("CoordinateToName: retrieve url " << urlStream.str());
  RetrieveXML(urlStream);
  ParseXMLGeonames();

  return true;
}

/*
//This method will be necessary to process the file directly in memory
//without writing it to the disk. Waiting for the xml lib to handle that
//also
static size_t
curlHandlerWriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
  void *data)
{
  register int realsize = (int)(size * nmemb);

  std::vector<char> *vec
    = static_cast<std::vector<char>*>(data);
  const char* chPtr = static_cast<char*>(ptr);
  vec->insert(vec->end(), chPtr, chPtr + realsize);

  return realsize;
}
*/

void CoordinateToName::RetrieveXML(std::ostringstream& urlStream)
{

  CURL *curl;
  CURLcode res;

  FILE* output_file = fopen(m_TempFileName.c_str(),"w");
  curl = curl_easy_init();

//   std::cout << "URL data " << urlStream.str().data() << std::endl;


  char url[256];
  strcpy(url,urlStream.str().data());

//   std::cout << url << std::endl;
  if (curl)
  {
    std::vector<char> chunk;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /*
    //Step needed to handle curl without temporary file
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,this->curlHandlerWriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_FILE, (void *)&chunk);
    */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);
    res = curl_easy_perform(curl);

    fclose(output_file);
    /* always cleanup */
    curl_easy_cleanup(curl);
  }

}


void CoordinateToName::ParseXMLGeonames()
{
  TiXmlDocument doc( m_TempFileName.c_str() );
  doc.LoadFile();
  TiXmlHandle docHandle( &doc );

  TiXmlElement* childName = docHandle.FirstChild( "geonames" ).FirstChild( "geoname" ).FirstChild( "name" ).Element();
  if ( childName )
  {
    m_PlaceName=childName->GetText();
  }
  TiXmlElement* childCountryName = docHandle.FirstChild( "geonames" ).FirstChild( "geoname" ).FirstChild( "countryName" ).Element();
  if ( childCountryName )
  {
    m_CountryName=childCountryName->GetText();
  }

  remove(m_TempFileName.c_str());
}

} // namespace otb

