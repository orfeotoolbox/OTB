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
#include "otbTinyXML.h"

namespace otb {


template < class TMeasurementVector >
StatisticsXMLFileReader<TMeasurementVector>
::StatisticsXMLFileReader(): m_FileName(""),
                             m_NumberOfOutputs(0),
                             m_IsUpdated(false)
{}

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
  for(unsigned int  idx = 0; idx < m_NumberOfOutputs; ++idx)
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

  // Open the xml file
  TiXmlDocument doc(m_FileName.c_str());
  if (!doc.LoadFile())
    {
    itkExceptionMacro(<<"Can't open file "<<m_FileName);
    }

  TiXmlHandle hDoc(&doc);
  TiXmlHandle root    = hDoc.FirstChildElement("FeatureStatistics");

  // Iterate through the tree to get all the stats
  for( TiXmlElement* currentStat = root.FirstChildElement().ToElement();
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

    // Increment the number of output
    m_MeasurementVectorContainer.push_back(currentStatisticVector);
    m_NumberOfOutputs++;
    }

  m_IsUpdated = true;
}


template < class TMeasurementVector >
void
StatisticsXMLFileReader<TMeasurementVector>
::PrintSelf(std::ostream& itkNotUsed(os), itk::Indent itkNotUsed(indent)) const
{
  // Call superclass implementation
  //Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
