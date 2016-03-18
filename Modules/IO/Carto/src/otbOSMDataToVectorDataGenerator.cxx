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

#include "otbOSMDataToVectorDataGenerator.h"
#include "otbGeoInformationConversion.h"
#include "itksys/SystemTools.hxx"
#include "itkDataObject.h"
#include "itkMacro.h"

#include "otb_tinyxml.h"

namespace otb
{

OSMDataToVectorDataGenerator::OSMDataToVectorDataGenerator():m_North(43.62811),
                                                             m_South(43.60185),
                                                             m_East(1.59323),
                                                             m_West(1.54911),
                                                             m_UseUrl(true)
{
  this->SetNumberOfRequiredOutputs(1);

  // Initialize the url
  m_Url = "http://www.openstreetmap.org/api/0.6/map?";

  m_Curl = CurlHelper::New();

  m_ClassKey = "Class";
  m_ClassKeyValue = 1;


  // Add the key to search by default
  m_KeyList.push_back("highway");
  m_KeyList.push_back("building");
  m_KeyList.push_back("landuse");
  m_KeyList.push_back("waterway");
  m_KeyList.push_back("cycleway");
  m_KeyList.push_back("public_transport");
  m_KeyList.push_back("railway");
  m_KeyList.push_back("natural");
}

OSMDataToVectorDataGenerator::~OSMDataToVectorDataGenerator()
{
}


void OSMDataToVectorDataGenerator::GenerateData()
{
  // Priority to the URL, if not set check if the filename is
  // used
  if(m_UseUrl)
    {
    // Get the url
    std::ostringstream urlStream;
    urlStream<<m_Url;

    // TODO : Replace by the new method RetrieveFileFromInMemory
    m_FileName = "temposmresult.xml";

    // Add the extent to the url
    urlStream<<"bbox="<<m_West<<","<<m_South<<","<<m_East<<","<<m_North;

    // First check if the request does not cross the server
    // limitations :
    if (m_Curl->IsCurlReturnHttpError( urlStream.str()))
      {
      itkExceptionMacro(<<"The OSM Server returned an Error > =400,"
                        <<" it means that one of server limits are crossed : node/way/relation or area requested");
      }

    // Use Curl to request the OSM Server
    // TODO use the RetrieveUrlInMemory
    m_Curl->RetrieveFile(urlStream.str(), m_FileName);
    }
  else
    {
    // Check if the filename is not empty
    if(m_FileName.empty())
      itkExceptionMacro(<<"The XML output FileName is empty, please set the filename via the method SetFileName");

    //Check that the right extension is given : expected .xml */
    if (itksys::SystemTools::GetFilenameLastExtension(m_FileName) != ".xml"
        &&  itksys::SystemTools::GetFilenameLastExtension(m_FileName)!= ".osm" )
      {
      itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_FileName)
                        <<" is a wrong Extension FileName : Expected .xml or osm");
      }
    }

  // Parse the XML File
  this->ParseXmlFile();

  // Remove the osm temp file only if the url request is used
  if( m_UseUrl)
    if(std::remove(m_FileName.c_str()) != 0)
      {
      itkExceptionMacro(<< "Error while deleting the file" << m_FileName);
      }
}

void OSMDataToVectorDataGenerator::ParseXmlFile()
{
  // Open the xml file
  TiXmlDocument doc(m_FileName.c_str());

  if (!doc.LoadFile())
    {
    itkExceptionMacro( <<"Cannot open the file "<<m_FileName );
    }

  // Parse the xml
  TiXmlHandle hDoc(&doc);
  TiXmlHandle root      = hDoc.FirstChildElement();

  if(!root.ToElement())
    itkExceptionMacro( "Invalid or corrupted XML file : The field way is not found");


  ////////////  ////////////  ////////////  ////////////  ////////////
  /*   Step 1 : Store the nodes in a std::map                       */
  ////////////  ////////////  ////////////  ////////////  ////////////

  // Iterate through the tree to get all the nodes and store them in
  // a std::map
  for( TiXmlElement* node = root.FirstChild("node").ToElement();
       node != NULL  && strcmp(node->Value(),"node")==0;
       node = node->NextSiblingElement() )
    {
    int id = 0;
    double longitude = 0.;
    double latitude = 0.;

    if ( node->QueryIntAttribute("id", &id) == TIXML_SUCCESS
         && node->QueryDoubleAttribute("lat", &latitude) == TIXML_SUCCESS
         && node->QueryDoubleAttribute("lon", &longitude) == TIXML_SUCCESS )
      {
      // current point
      VertexType geoPoint;
      geoPoint[0] = longitude;
      geoPoint[1] = latitude;

      // std::pair to add in the map
      std::pair<int, VertexType> newEntry;
      newEntry.first  = id;
      newEntry.second = geoPoint;
      m_GeoPointContainer.insert(newEntry);
      }
    }
  ////////////  ////////////  ////////////  ////////////  ////////////
  /*   Step 2 : Find the Key and the node ids of the key */
  ////////////  ////////////  ////////////  ////////////  ////////////

  // Iterate through the tree to get all the fields ways with tag k=key
  for( TiXmlElement* currentLayer = root.FirstChildElement("way").ToElement();
       currentLayer != NULL && strcmp(currentLayer->Value(),"way") ==0;
       currentLayer = currentLayer->NextSiblingElement() )
    {
    bool foundKey = false;

    // Is the element "way" have a child tag
    if(currentLayer->FirstChildElement("tag"))
      {
      ElementPairType         elementPair;

      // Iterate through the node tag to find the "key"
      for( TiXmlElement* currentTag = currentLayer->FirstChild("tag")->ToElement();
           currentTag != NULL;
           currentTag = currentTag->NextSiblingElement() )
        {
        std::string result = currentTag->Attribute("k");

        if(this->IsKeyPresent(result))
          {
          // Set the flag to true;
          foundKey = true;

          // Get the value of the key
          std::string resultValue = currentTag->Attribute("v");

          // Fill the element information
          elementPair.first  = result;
          elementPair.second = resultValue;

          // Add the key, value to map
          this->AddKeyTypeToMap(result, resultValue);

          break;
          }
        }

      if(foundKey)
        {
        VectorDataElementType   vdelement;
        PointTypeList           pointList;

        // put the current layer pointer to the begining
        for( TiXmlElement* currentNode = currentLayer->FirstChildElement("nd");
             currentNode != NULL;
             currentNode = currentNode->NextSiblingElement() )
          {
          int value=0;
          if(currentNode->QueryIntAttribute("ref", &value) == TIXML_SUCCESS)
            {
            // Add the current point to the list
            pointList.push_back((*m_GeoPointContainer.find(value)).second);
            }
          }

        // Form the VectorDataElement
        if(pointList.size() > 0)
          {
          vdelement.first  = elementPair;
          vdelement.second = pointList;
          m_VectorDataElementList.push_back(vdelement);
          }
        }

      } // end if tag k exists
    }
}

void
OSMDataToVectorDataGenerator::AddKeyTypeToMap(const std::string& key, const std::string& value)
{
  // Is this key present in the map
  KeyMapType::iterator   it = m_KeysMap.find(key);

  if( it != m_KeysMap.end())
    {
    // vector containing the types relative to this key
    StringVectorType &currentTypes = (*it).second;

    bool found = false;

    // Add the type if not present
    for(unsigned int i = 0; i < currentTypes.size(); ++i)
      {
      if(currentTypes[i].compare(value)  == 0)
        found = true;
      }
    // if the value was not found add it the currentypes vector
    if(!found)
      {
      currentTypes.push_back(value);
      }
    }
  else
    {
    std::pair< std::string, StringVectorType > keytype;
    keytype.first  = key;
    keytype.second.push_back(value);
    m_KeysMap.insert(keytype);
    }
}

void
OSMDataToVectorDataGenerator::ProcessVectorData(const std::string& key, const std::string& value)
{
  // the output
  m_OutputVectorData = VectorDataType::New();

  // Intitialize the vectordata : EPSG 4326 is for WGS84
  m_OutputVectorData->SetProjectionRef(otb::GeoInformationConversion::ToWKT(4326));
  DataNodeType::Pointer root = m_OutputVectorData->GetDataTree()->GetRoot()->Get();
  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder = DataNodeType::New();

  document->SetNodeType(otb::DOCUMENT);
  folder->SetNodeType(otb::FOLDER);

  document->SetNodeId("DOCUMENT");
  folder->SetNodeId("FOLDER");

  m_OutputVectorData->GetDataTree()->Add(document, root);
  m_OutputVectorData->GetDataTree()->Add(folder, document);

  // Form the vector data with the values parsed
  for(unsigned int idx = 0; idx < m_VectorDataElementList.size(); ++idx)
    {
    // Instanciate a datanode
    DataNodeType::Pointer currentDataNode = DataNodeType::New();

    // Get the current PointTypeList
    PointTypeList   currentPointList = m_VectorDataElementList[idx].second;

    // Get the current ElementPairList
    ElementPairType  elementPair    = m_VectorDataElementList[idx].first;

    // Add it to the vector data only if the requested key matches to
    // elementPair key
    if(elementPair.first.compare(key) == 0)
      {

      // value is empty
      // or if it is not empty it has to match the requestd value
      if(value.empty() || elementPair.second.compare(value) == 0)
        {

        // If the first and the last vertex are the same = polygon else
        // line
        if(currentPointList[0].EuclideanDistanceTo(currentPointList[currentPointList.size()-1]) < 1e-10) // polygon
          {
          currentDataNode->SetNodeId("FEATURE_POLYGON");
          currentDataNode->SetNodeType(otb::FEATURE_POLYGON);
          PolygonType::Pointer polygon = PolygonType::New();
          for(unsigned int curVertexId = 0; curVertexId< currentPointList.size(); curVertexId++)
            {
            polygon->AddVertex(currentPointList[curVertexId]);
            }
          currentDataNode->SetPolygonExteriorRing(polygon);
          }
        else  // line
          {
          LineType::Pointer currentLine = LineType::New();
          for(unsigned int curVertexId = 0; curVertexId< currentPointList.size(); curVertexId++)
            {
            currentLine->AddVertex(currentPointList[curVertexId]);
            }
          currentDataNode->SetNodeId("FEATURE_LINE");
          currentDataNode->SetNodeType(otb::FEATURE_LINE);
          currentDataNode->SetLine(currentLine);
          }

        // Add some fields (TODO : mapnik necessary fields for rendering )
        currentDataNode->SetFieldAsString("Key", elementPair.first);
        currentDataNode->SetFieldAsString("type", elementPair.second);
        currentDataNode->SetFieldAsInt(m_ClassKey, m_ClassKeyValue);

        // Add the to vectordata
        m_OutputVectorData->GetDataTree()->Add(currentDataNode, folder);
        }
      }
    }
}

const OSMDataToVectorDataGenerator::VectorDataType*
OSMDataToVectorDataGenerator::GetVectorDataByName(const std::string& key)
{
  if(key.empty())
    itkExceptionMacro(<<"Requested key is empty, please set a valid one");

  // First thing to do is to add the key set as parameters in the
  // KeyList
  if(!this->IsKeyPresent(key))
    {
    // TODO leave this functionnality for AddKey ?
    m_KeyList.push_back(key);
    this->ParseXmlFile();
    }

  // Build the output
  this->ProcessVectorData(key,"");

  // Get the output
  return m_OutputVectorData;
}

const OSMDataToVectorDataGenerator::VectorDataType*
OSMDataToVectorDataGenerator::GetVectorDataByName(const std::string& key, const std::string& value)
{
  if(key.empty())
    itkExceptionMacro(<<"Requested key is empty, please set a valid one");

  // Add the key set as parameters in the KeyList
  // TODO leave this functionnality for AddKey ?
  if(!this->IsKeyPresent(key))
    {
    m_KeyList.push_back(key);
    this->ParseXmlFile();
    }

  // Build the output
  this->ProcessVectorData(key, value);

  // Get the output
  return m_OutputVectorData;
}


bool
OSMDataToVectorDataGenerator::IsKeyPresent(const std::string& key)
{
  for(unsigned int i = 0; i < m_KeyList.size(); ++i)
    if(m_KeyList[i].compare(key) == 0)
      return true;

  // Otherwise return false;
  return false;
}


}
