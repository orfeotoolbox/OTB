/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperOutputVectorDataParameter_h
#define otbWrapperOutputVectorDataParameter_h

#include "otbWrapperParameter.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include <string>

namespace otb
{
namespace Wrapper
{
/** \class OutputVectorDataParameter
 *  \brief This class represents a OutputVectorData parameter
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT OutputVectorDataParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputVectorDataParameter     Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputVectorDataParameter, Parameter);


  /** Set the value */
  itkSetObjectMacro(VectorData, VectorDataType);

  /** Get the value */
  itkGetObjectMacro(VectorData, VectorDataType);

  /** Return true if a filename is set */
  bool HasValue() const override;

  /** Return any value */
  void SetValue(VectorDataType* vd);

  /** Return any value */
  VectorDataType* GetValue(void);

  void SetFileName(const char* filename);
  void SetFileName(const std::string& filename);

  itkGetStringMacro(FileName);

  void                Write();
  itk::ProcessObject* GetWriter();
  void                InitializeWriters();

  ParameterType GetType() const override
  {
    return ParameterType_OutputVectorData;
  }

  std::string ToString() const override
  {
    return GetFileName();
  }

  void FromString(const std::string& value) override
  {
    SetFileName(value);
  }

protected:
  /** Constructor */
  OutputVectorDataParameter();

  /** Destructor */
  ~OutputVectorDataParameter() override
  {
  }


  VectorDataType::Pointer m_VectorData;
  std::string             m_FileName;

  otb::VectorDataFileWriter<VectorDataType>::Pointer m_Writer;

private:
  OutputVectorDataParameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;
};

} // End namespace Wrapper
} // End namespace otb

#endif
