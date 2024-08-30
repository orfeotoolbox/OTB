/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperFieldParameter_h
#define otbWrapperFieldParameter_h

#include <string>
#include "otbWrapperListViewParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class FieldParameter
 *  \brief This class represents a field parameter selected from a vector file
 * for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT FieldParameter : public ListViewParameter
{
public:
  /** Standard class typedef */
  typedef FieldParameter                Self;
  typedef ListViewParameter               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(FieldParameter, ListViewParameter);

  typedef std::vector<OGRFieldType> TypeFilterType;

  ParameterType GetType() const override
  {
    return ParameterType_Field;
  }

  /** Set vector data name from which we choose the fields */
  void SetVectorData(std::string vectorData)
  {
    m_VectorData = std::move(vectorData);
  }

  /** Get vector data name from which we choose the fields */
  const std::string& GetVectorData() const
  {
    return m_VectorData;
  }

  /** Set list of allowed field types */
  void SetTypeFilter(TypeFilterType typeFilter)
  {
    m_TypeFilter = std::move(typeFilter);
  }

  /** Get list of allowed field types */
  const TypeFilterType& GetTypeFilter() const
  {
    return m_TypeFilter;
  }

protected:
  /** Constructor */
  FieldParameter() = default;

  /** Destructor */
  ~FieldParameter() override = default;

  std::string m_VectorData;
  TypeFilterType m_TypeFilter;

private:
  FieldParameter(const FieldParameter&) = delete;
  void operator=(const FieldParameter&) = delete;

}; // End class FieldParameter

} // End namespace Wrapper
} // End namespace otb

#endif
