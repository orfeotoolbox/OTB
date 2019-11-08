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

#ifndef otbStatisticsXMLFileWriter_h
#define otbStatisticsXMLFileWriter_h

#include "itkProcessObject.h"
#include <utility>
#include <string>

namespace otb
{

/** \class StatisticsXMLFileWriter
 *  \brief Write in a xml file the values stored in a MeasurementVector set as
 *  input
 *
 * The vector can be set as input via AddInput(name, vector) where name
 * is the name of the statistic, and vector the values.
 * Supported vector types are those implementing the method GetElement(idx)
 * and defining the type ValueType.
 *
 *
 * \ingroup OTBIOXML
 */
template <class TMeasurementVector>
class StatisticsXMLFileWriter : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef StatisticsXMLFileWriter       Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(StatisticsXMLFileWriter, itk::Object);

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

  /** Method to set/get the input list sample */
  void AddInput(const char* name, const MeasurementVectorType& inputVector);

  /** Method to add a map statistic with a given type */
  template <typename MapType>
  void AddInputMap(const char* name, const MapType& map);

  /** Remove previously added inputs (vectors and maps) */
  void CleanInputs();

  /** Trigger the processing */
  void Update()
  {
    this->GenerateData();
  }

  /** Set the output filename */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:
  virtual void GenerateData();

  StatisticsXMLFileWriter();
  ~StatisticsXMLFileWriter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  StatisticsXMLFileWriter(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string                m_FileName;
  MeasurementVectorContainer m_MeasurementVectorContainer;
  GenericMapContainer        m_GenericMapContainer;

}; // end of class StatisticsXMLFileWriter

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStatisticsXMLFileWriter.hxx"
#endif

#endif
