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
#ifndef __otbStatisticsXMLFileReader_txx
#define __otbStatisticsXMLFileReader_txx

#include "otbStatisticsXMLFileReader.h"
#include "itkMacro.h"
#include "itksys/SystemTools.hxx"
#include "otb_tinyxml.h"
#include "otbStringUtils.h"

namespace otb {


template < class TMeasurementVector >
StatisticsXMLFileReader<TMeasurementVector>
::StatisticsXMLFileReader(): m_FileName(""),
                             m_IsUpdated(false)
{}

template < class TMeasurementVector >
unsigned int
StatisticsXMLFileReader<TMeasurementVector>
::GetNumberOfOutputs()
{
  return m_MeasurementVectorContainer.size() + m_GenericMapContainer.size();
}

template < class TMeasurementVector >
std::vector<std::string>
StatisticsXMLFileReader<TMeasurementVector>
::GetStatisticVectorNames()
{
  // Read the xml file once
  if(!m_IsUpdated)
    {
    this->Read();
    }

  std::vector<std::string> output;
  for (unsigned int i=0 ; i < m_MeasurementVectorContainer.size() ; ++i )
    {
    output.push_back(m_MeasurementVectorContainer[i].first);
    }
  return output;
}

template < class TMeasurementVector >
std::vector<std::string>
StatisticsXMLFileReader<TMeasurementVector>
::GetStatisticMapNames()
{
  // Read the xml file once
  if(!m_IsUpdated)
    {
    this->Read();
    }

  std::vector<std::string> output;
  for ( GenericMapContainer::iterator it = m_GenericMapContainer.begin() ;
        it != m_GenericMapContainer.end() ;
        ++it)
    {
    output.push_back(it->first);
    }
  return output;
}

template < class TMeasurementVector >
typename StatisticsXMLFileReader<TMeasurementVector>
::MeasurementVectorType
StatisticsXMLFileReader<TMeasurementVector>
::GetStatisticVectorByName(const char * statisticName)
{
  // Read the xml file once
  if(!m_IsUpdated)
    {
    this->Read();
    }

  // Check if the name of the Statistic is present
  bool found = false;
  unsigned int index = 0;
  for(unsigned int  idx = 0; idx < m_MeasurementVectorContainer.size() ; ++idx)
    {
    if(strcmp(m_MeasurementVectorContainer[idx].first.c_str(), statisticName) == 0 )
      {
      found = true;
      index = idx;
      }
    }

  // if token not found throw an axception
  if(!found)
    itkExceptionMacro(<<"No entry corresponding to the token selected ("<<statisticName<<") in the XML file");

  return m_MeasurementVectorContainer[index].second;
}

template < class TMeasurementVector >
template <typename MapType>
MapType
StatisticsXMLFileReader<TMeasurementVector>
::GetStatisticMapByName(const char * statisticName)
{
  // Read the xml file once
  if(!m_IsUpdated)
    {
    this->Read();
    }

  // Check if the name of the Statistic is present
  std::string statName(statisticName);
  if (m_GenericMapContainer.count(statName) == 0)
    {
    itkExceptionMacro(<<"No entry corresponding to the token selected ("<<statName<<") in the XML file");
    }

  MapType outputMap;
  typename MapType::key_type tmpKey;
  typename MapType::mapped_type tmpVal;
  for ( GenericMapType::iterator it = m_GenericMapContainer[statName].begin() ;
        it != m_GenericMapContainer[statName].end() ;
        ++it)
    {
    tmpKey = boost::lexical_cast<typename MapType::key_type>(it->first);
    tmpVal = boost::lexical_cast<typename MapType::mapped_type>(it->second);
    outputMap[tmpKey] = tmpVal;
    }
  return outputMap;
}

template < class TMeasurementVector >
void
StatisticsXMLFileReader<TMeasurementVector>
::Read()
{
  // Check if the filename is not empty
  if(m_FileName.empty())
    itkExceptionMacro(<<"The XML output FileName is empty, please set the filename via the method SetFileName");

  // Check that the right extension is given : expected .xml */
  if (itksys::SystemTools::GetFilenameLastExtension(m_FileName) != ".xml")
    {
    itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_FileName)
                      <<" is a wrong Extension FileName : Expected .xml");
    }

  // Clean outputs
  m_MeasurementVectorContainer.clear();
  m_GenericMapContainer.clear();

  // Open the xml file
  TiXmlDocument doc(m_FileName.c_str());
  if (!doc.LoadFile())
    {
    itkExceptionMacro(<<"Can't open file "<<m_FileName);
    }

  TiXmlHandle hDoc(&doc);
  TiXmlElement *root    = hDoc.FirstChildElement("FeatureStatistics").ToElement();

  if (root)
    {
    // Iterate through the tree to get all the stats
    for( TiXmlElement* currentStat = root->FirstChildElement();
         currentStat != NULL;
         currentStat = currentStat->NextSiblingElement() )
      {
      InputDataType   currentStatisticVector;

      // Store the stat type name
      currentStatisticVector.first = currentStat->Attribute("name");

      // The size is not stored in the XML file
      // Store the value in a std::vector, get the size and then
      // build a measurement vector
      std::vector<double>   tempMeasurementVector;

      for( TiXmlElement* sample = currentStat->FirstChildElement("StatisticVector");
     sample != NULL;
     sample = sample->NextSiblingElement() )
        {
        // Get the current value of the statistic vector
        double value;
        sample->QueryDoubleAttribute("value", &value);
        // Store the value
        tempMeasurementVector.push_back(value);
        }

      // resize the Measurement Vector
      currentStatisticVector.second.SetSize(tempMeasurementVector.size());
      for(unsigned int i = 0; i < tempMeasurementVector.size(); ++i)
        currentStatisticVector.second.SetElement(i,
                                                 (static_cast<InputValueType>(tempMeasurementVector[i])));
      m_MeasurementVectorContainer.push_back(currentStatisticVector);
      }
    }

  // Parse Map statistics
  std::string key;
  std::string value;
  root = hDoc.FirstChildElement("GeneralStatistics").ToElement();
  if (root)
    {
    // Iterate through the tree to get all the stats
    for( TiXmlElement* currentStat = root->FirstChildElement();
         currentStat != NULL;
         currentStat = currentStat->NextSiblingElement() )
      {
      GenericMapType currentMap;
      std::string currentName(currentStat->Attribute("name"));

      for( TiXmlElement* sample = currentStat->FirstChildElement("StatisticMap");
          sample != NULL;
          sample = sample->NextSiblingElement() )
        {
        // Get the current pair of the statistic map
        const char *c_key = sample->Attribute("key");
        const char *c_value = sample->Attribute("value");
        if (c_key == NULL)
          {
          itkExceptionMacro("'key' attribute not found in StatisticMap !");
          }
        if (c_value == NULL)
          {
          itkExceptionMacro("'value' attribute not found in StatisticMap !");
          }
        key = std::string(c_key);
        value = std::string(c_value);
        // Store the pair
        currentMap[key] = value;
        }
      m_GenericMapContainer[currentName] = currentMap;
      }
    }
  // Reader is up-to-date
  m_IsUpdated = true;
}


template < class TMeasurementVector >
void
StatisticsXMLFileReader<TMeasurementVector>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);

  // Print info about statistics
  os << indent << "Input FileName: "<< m_FileName << std::endl;
  os << indent << "Vector statistics: ";
  for (unsigned int i=0 ; i < m_MeasurementVectorContainer.size() ; ++i)
    {
    if (i>0) os <<", ";
    os << m_MeasurementVectorContainer[i].first;
    }
  os << std::endl;
  os << indent << "Map statistics: ";
  for (GenericMapContainer::const_iterator it = m_GenericMapContainer.begin() ; it != m_GenericMapContainer.end() ; ++it)
    {
    if (it != m_GenericMapContainer.begin()) os <<", ";
    os << it->first;
    }
  os << std::endl;
}

} // End namespace otb

#endif
