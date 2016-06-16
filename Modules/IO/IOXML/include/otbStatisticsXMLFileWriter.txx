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
#ifndef otbStatisticsXMLFileWriter_txx
#define otbStatisticsXMLFileWriter_txx

#include "otbStatisticsXMLFileWriter.h"
#include "itkMacro.h"
#include "itksys/SystemTools.hxx"
#include "otb_tinyxml.h"
#include "otbStringUtils.h"

namespace otb {


template < class TMeasurementVector >
StatisticsXMLFileWriter<TMeasurementVector>
::StatisticsXMLFileWriter(): m_FileName("")
{}


template < class TMeasurementVector >
void
StatisticsXMLFileWriter<TMeasurementVector>
::AddInput(const char * name,  const MeasurementVectorType& inputVector )
{
  InputDataType    inputData;
  inputData.first  = name;

  // Check if the statistic name is already added
  for(unsigned int idx= 0; idx< m_MeasurementVectorContainer.size(); ++idx)
    {
    if(strcmp(m_MeasurementVectorContainer[idx].first.c_str(), name) == 0 )
      {
      itkExceptionMacro(<<"Token selected ("
                        <<name<<") is already added to the XML file");
      }
    }

  inputData.second = inputVector;
  m_MeasurementVectorContainer.push_back(inputData);
}

template < class TMeasurementVector >
void
StatisticsXMLFileWriter<TMeasurementVector>
::GenerateData()
{
  // Check if the input are not null
  if(m_MeasurementVectorContainer.size() == 0 && m_GenericMapContainer.size() == 0)
    itkExceptionMacro(<<"At least one input is required, please set input using the methods AddInput or AddInputMap");

  // Check if the filename is not empty
  if(m_FileName.empty())
    itkExceptionMacro(<<"The XML output FileName is empty, please set the filename via the method SetFileName");

  // Check that the right extension is given : expected .xml */
  if (itksys::SystemTools::GetFilenameLastExtension(m_FileName) != ".xml")
    {
    itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_FileName)
                      <<" is a wrong Extension FileName : Expected .xml");
    }

  // Write the XML file
  TiXmlDocument doc;

  TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
  doc.LinkEndChild( decl );

  TiXmlElement * root = ITK_NULLPTR;
  if (m_MeasurementVectorContainer.size())
    {
    root = new TiXmlElement( "FeatureStatistics");
    doc.LinkEndChild( root );
    }

  // Iterate through the input
  for (unsigned int i = 0; i < m_MeasurementVectorContainer.size(); ++i)
    {
    std::string            featureName              = m_MeasurementVectorContainer[i].first;
    MeasurementVectorType  currentMeasurementVector = m_MeasurementVectorContainer[i].second;

    // The current statistic
    TiXmlElement * feature = new TiXmlElement("Statistic");
    feature->SetAttribute("name", featureName.c_str());
    root->LinkEndChild( feature );

    // Store the value for this statistic
    for(unsigned int cindex = 0; cindex < currentMeasurementVector.Size(); ++cindex)
      {
      // For each value in Measurementvector
      TiXmlElement * curStatisticVector = new TiXmlElement("StatisticVector");
      curStatisticVector->SetDoubleAttribute("value", currentMeasurementVector.GetElement(cindex));
      feature->LinkEndChild(curStatisticVector);
      }
    }
    
  // Iterate on map containers
  TiXmlElement * mapRoot = ITK_NULLPTR;
  if (m_GenericMapContainer.size())
    {
    mapRoot = new TiXmlElement( "GeneralStatistics");
    doc.LinkEndChild( mapRoot );
    }

  GenericMapContainer::const_iterator containerIt;
  for ( containerIt = m_GenericMapContainer.begin() ; containerIt != m_GenericMapContainer.end() ; ++containerIt)
    {
    std::string mapName = containerIt->first;
    GenericMapType::const_iterator mapIter;

    // The current statistic
    TiXmlElement * feature = new TiXmlElement("Statistic");
    feature->SetAttribute("name", mapName.c_str());
    mapRoot->LinkEndChild( feature );

    // Store the value for this statistic
    for( mapIter = containerIt->second.begin() ; mapIter != containerIt->second.end() ; ++mapIter )
      {
      // For each value in Measurementvector
      TiXmlElement * curStatisticMap = new TiXmlElement("StatisticMap");
      curStatisticMap->SetAttribute("key" , mapIter->first.c_str());
      curStatisticMap->SetAttribute("value", mapIter->second.c_str());
      feature->LinkEndChild(curStatisticMap);
      }
    }
  

  // Finally, write the file
  if (! doc.SaveFile( m_FileName.c_str() ) )
    {
    itkExceptionMacro(<<"Unable to write the XML file in "
                      << itksys::SystemTools::GetFilenamePath(m_FileName)
                      << " (permission issue? Directory does not exist?)." );
    }

}

template < class TMeasurementVector >
template <typename MapType>
void
StatisticsXMLFileWriter<TMeasurementVector>
::AddInputMap(const char * name, const MapType& map )
{
  std::string token(name);
  
  if(m_GenericMapContainer.count(token) > 0)
    {
    itkExceptionMacro(<<"Token selected ("
                      <<name<<") is already added to the XML file");
    }
  
  typename MapType::const_iterator it;
  GenericMapType insideMap;
  std::string tmpKey;
  std::string tmpVal;
  for ( it = map.begin() ; it != map.end() ; ++it)
    {
    tmpKey = boost::lexical_cast<std::string>(it->first);
    tmpVal = boost::lexical_cast<std::string>(it->second);
    insideMap[tmpKey] = tmpVal;
    }
  m_GenericMapContainer[token] = insideMap;
}

template < class TMeasurementVector >
void
StatisticsXMLFileWriter<TMeasurementVector>
::CleanInputs()
{
  // clear both containers
  m_MeasurementVectorContainer.clear();
  m_GenericMapContainer.clear();
}

template < class TMeasurementVector >
void
StatisticsXMLFileWriter<TMeasurementVector>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);

  // Print Writer state
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
