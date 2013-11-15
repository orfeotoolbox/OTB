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
#ifndef __otbStatisticsXMLFileWriter_txx
#define __otbStatisticsXMLFileWriter_txx

#include "otbStatisticsXMLFileWriter.h"
#include "itkMacro.h"
#include "itksys/SystemTools.hxx"
#include "otbTinyXML.h"

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
  if(m_MeasurementVectorContainer.size() == 0)
    itkExceptionMacro(<<"At Least one input is required, please set input using the method AddInput");

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

  TiXmlElement * root = new TiXmlElement( "FeatureStatistics");
  doc.LinkEndChild( root );

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

  // Finally, write the file
  doc.SaveFile( m_FileName.c_str() );
}


template < class TMeasurementVector >
void
StatisticsXMLFileWriter<TMeasurementVector>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  //Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
