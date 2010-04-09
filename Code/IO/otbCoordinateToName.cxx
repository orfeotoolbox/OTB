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
#include "otbMacro.h"
#include <sstream>

#ifdef OTB_USE_CURL
#include "tinyxml.h"
#include <curl/curl.h>
#endif

#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{

/**
 * Constructor
 */

CoordinateToName::CoordinateToName() :
  m_Lon(-1000.0), m_Lat(-1000.0), m_Multithread(false), m_IsValid(false)
{
  m_PlaceName = "";
  m_CountryName = "";

  //Avoid collision between different instance of the class
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGenType;
  RandomGenType::Pointer randomGen = RandomGenType::GetInstance();
  randomGen->SetSeed(reinterpret_cast<long int>(this)); //make sure the seed is unique for this class
  int randomNum = randomGen->GetIntegerVariate();

  std::stringstream filename;
  filename << "tmp-coordinateToName-SignayriUt1-";
  filename << randomNum;
  filename << ".xml";
  m_TempFileName = filename.str();

  m_Threader = itk::MultiThreader::New();

  m_UpdateDistance = 0.01; //about 1km at equator

}

/**
 * PrintSelf
 */
void
CoordinateToName
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " m_Lon "  << m_Lon << std::endl;
  os << indent << " m_Lat "  << m_Lat << std::endl;
  os << indent << " m_PlaceName "  << m_PlaceName << std::endl;
}

bool CoordinateToName::Evaluate()
{
  if (m_Multithread)
    {
    m_Threader->SpawnThread(ThreadFunction, this);
    }
  else
    {
    DoEvaluate();
    }
  return true;
}

ITK_THREAD_RETURN_TYPE
CoordinateToName::ThreadFunction(void *arg)
{
  struct itk::MultiThreader::ThreadInfoStruct * pInfo = (itk::MultiThreader::ThreadInfoStruct *) (arg);
  CoordinateToName::Pointer  lThis = (CoordinateToName*) (pInfo->UserData);
  lThis->DoEvaluate();
  return 0;
}

void CoordinateToName::DoEvaluate()
{
  std::ostringstream urlStream;
  urlStream << "http://ws.geonames.org/findNearbyPlaceName?lat=";
  urlStream << m_Lat;
  urlStream << "&lng=";
  urlStream << m_Lon;
  otbMsgDevMacro("CoordinateToName: retrieve url " << urlStream.str());
  RetrieveXML(urlStream);
  std::string placeName = "";
  std::string countryName = "";
  ParseXMLGeonames(placeName, countryName);
  m_PlaceName = placeName;
  m_CountryName = countryName;
  m_IsValid = true;
}

void CoordinateToName::RetrieveXML(std::ostringstream& urlStream) const
{
#ifdef OTB_USE_CURL
  CURL *   curl;
  CURLcode res;

  FILE* output_file = fopen(m_TempFileName.c_str(), "w");
  curl = curl_easy_init();

  char url[256];
  strcpy(url, urlStream.str().data());

//   std::cout << url << std::endl;
  if (curl)
    {
    std::vector<char> chunk;
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set 5s timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);
    res = curl_easy_perform(curl);

    fclose(output_file);
    /* always cleanup */
    curl_easy_cleanup(curl);
    }
#endif
}

void CoordinateToName::ParseXMLGeonames(std::string& placeName, std::string& countryName) const
{
#ifdef OTB_USE_CURL
  TiXmlDocument doc(m_TempFileName.c_str());
  doc.LoadFile();
  TiXmlHandle docHandle(&doc);

  TiXmlElement* childName = docHandle.FirstChild("geonames").FirstChild("geoname").
                            FirstChild("name").Element();
  if (childName)
    {
    placeName = childName->GetText();
    }
  TiXmlElement* childCountryName = docHandle.FirstChild("geonames").FirstChild("geoname").
                                   FirstChild("countryName").Element();
  if (childCountryName)
    {
    countryName = childCountryName->GetText();
    }
  otbMsgDevMacro(<< "Near " << placeName << " in " << countryName);
  remove(m_TempFileName.c_str());
#endif
}

} // namespace otb
