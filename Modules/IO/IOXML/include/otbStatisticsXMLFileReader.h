/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStatisticsXMLFileReader_h
#define otbStatisticsXMLFileReader_h

#include "itkProcessObject.h"
#include <string>

namespace otb
{

/** \class StatisticsXMLFileReader
 *  \brief   Read a xml file where are stored several statistics
 *
 * To get a specific statistic Measurement vector, use the method
 * GetStatisticVectorByName(name) which allow you to get the
 * Measurement vector for the statistic set as parameter.
 *
 *
 *
 * \ingroup OTBIOXML
 */
template <class TMeasurementVector>
class StatisticsXMLFileReader : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef StatisticsXMLFileReader       Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(StatisticsXMLFileReader, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputSampleList typedefs */
  typedef TMeasurementVector                        MeasurementVectorType;
  typedef typename MeasurementVectorType::ValueType InputValueType;

  /** Convenient typedef */
  typedef std::pair<std::string, MeasurementVectorType> InputDataType;
  typedef std::vector<InputDataType> MeasurementVectorContainer;

  typedef std::map<std::string, std::string>    GenericMapType;
  typedef std::map<std::string, GenericMapType> GenericMapContainer;

  void Modified() const override
  {
    m_IsUpdated = false;
  }

  /** Set the output filename */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Get the number of Outputs*/
  unsigned int GetNumberOfOutputs();

  /** Get the list of vector statistics names */
  std::vector<std::string> GetStatisticVectorNames();

  /** Get the list of map statistics names */
  std::vector<std::string> GetStatisticMapNames();

  /** Method to get the MeasurementVector by name */
  MeasurementVectorType GetStatisticVectorByName(const char* statisticName);

  /** Method to get a statistics map by name */
  template <typename MapType>
  MapType GetStatisticMapByName(const char* statisticName);

protected:
  /** Read into the file and extract information in vector and map containers */
  virtual void Read();

  StatisticsXMLFileReader();
  ~StatisticsXMLFileReader() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  StatisticsXMLFileReader(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string                m_FileName;
  MeasurementVectorContainer m_MeasurementVectorContainer;
  mutable bool               m_IsUpdated;
  GenericMapContainer        m_GenericMapContainer;

}; // end of class StatisticsXMLFileReader

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStatisticsXMLFileReader.hxx"
#endif

#endif
