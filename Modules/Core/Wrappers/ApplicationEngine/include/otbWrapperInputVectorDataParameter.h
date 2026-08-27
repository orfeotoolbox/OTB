/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperInputVectorDataParameter_h
#define otbWrapperInputVectorDataParameter_h

#include "otbVectorDataFileReader.h"
#include "otbWrapperParameter.h"
#include <string>

namespace otb
{
namespace Wrapper
{
/** \class InputVectorDataParameter
 *  \brief This class represents a InputVectorData parameter
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT InputVectorDataParameter : public Parameter
{
public:
  /** Standard class typedef */
  using Self         = InputVectorDataParameter;
  using Superclass   = Parameter;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputVectorDataParameter, Parameter);

  using CoordinatePrecisionType = double;
  using ValuePrecisionType      = double;
  using VectorDataType          = otb::VectorData<CoordinatePrecisionType, 2, ValuePrecisionType>;

  /** Set value from filename */
  bool SetFromFileName(std::string filename);
  itkGetConstReferenceMacro(FileName, std::string);

  const VectorDataType* GetVectorData() const;
  VectorDataType*       GetVectorData();

  void SetVectorData(VectorDataType* vectorData);

  bool HasValue() const override;

  void ClearValue() override;

  ParameterType GetType() const override
  {
    return ParameterType_InputVectorData;
  }

  std::string ToString() const override
  {
    return GetFileName();
  }

  void FromString(const std::string& value) override
  {
    SetFromFileName(value);
  }

protected:
  /** Constructor */
  InputVectorDataParameter();

  /** Destructor */
  ~InputVectorDataParameter() override = default;

  using VectorDataFileReaderType = otb::VectorDataFileReader<VectorDataType>;

  VectorDataType::Pointer                           m_VectorData;
  VectorDataFileReaderType::Pointer                 m_Reader;

  std::string m_FileName;
  std::string m_PreviousFileName;

private:
  InputVectorDataParameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;
};

} // End namespace Wrapper
} // End namespace otb

#endif
