/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2018 CS Systemes d'Information (CS SI)
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



#include <sstream>

#include "otbCoordinateToName.h"
#include "otbMacro.h"
#include "otbUtils.h"

#include "otb_tinyxml.h"
#include "otbCurlHelper.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{

/**
 * Constructor
 */
CoordinateToName::CoordinateToName() :
  m_Lon(-1000.0), m_Lat(-1000.0), m_Multithread(false), m_IsValid(false),
  m_PlaceName(""), m_CountryName("")
{
  /*
  //Avoid collision between different instance of the class
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGenType;
  RandomGenType::Pointer randomGen = RandomGenType::GetInstance();
  randomGen->SetSeed(reinterpret_cast<long int>(this)); //make sure the seed is unique for this class
  unsigned int randomNum = randomGen->GetIntegerVariate();

  std::stringstream filename;
  filename << "tmp-coordinateToName-SignayriUt1-";
  filename << randomNum;
  filename << ".xml";
  m_TempFileName = filename.str();
  */

  m_Curl = CurlHelper::New();

  m_Threader = PlatformMultiThreader::New();

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
    /* SpawnThread should be replace by std::thread in future */
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
  return ITK_THREAD_RETURN_VALUE;
}

void CoordinateToName::DoEvaluate()
{
    m_PlaceName = "";
    m_CountryName = "";
    m_IsValid = false;

  if (Utils::IsLonLatValid(m_Lon, m_Lat))
    {
    std::ostringstream urlStream;
    urlStream << "http://api.geonames.org/findNearbyPlaceName?lat=";
    urlStream << m_Lat;
    urlStream << "&lng=";
    urlStream << m_Lon;
    urlStream << "&username=otbteam";
    otbMsgDevMacro("CoordinateToName: retrieve url " << urlStream.str());

    try
      {
      m_Curl->RetrieveUrlInMemory(urlStream.str(), m_CurlOutput);
      m_IsValid = true;
      }
    catch( itk::ExceptionObject & )
      {
      m_IsValid = false;
      }

    if(m_IsValid)
      {
      std::string placeName = "";
      std::string countryName = "";
      ParseXMLGeonames(placeName, countryName);
      m_PlaceName = placeName;
      m_CountryName = countryName;
      }
    }
}

void CoordinateToName::ParseXMLGeonames(std::string& placeName, std::string& countryName) const
{
  TiXmlDocument doc;
  doc.Parse(m_CurlOutput.c_str());

  if (!doc.Error())
    {
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
    }
}

} // namespace otb
