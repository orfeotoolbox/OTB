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

#ifndef otbWrapperInputProcessXMLParameter_h
#define otbWrapperInputProcessXMLParameter_h
#include "otbWrapperApplication.h"
#include "otb_tinyxml.h"
#include <sstream>
#include <string>

namespace otb
{
namespace Wrapper
{

/** \class InputProcessXMLParameter
 *  \brief This class represent a xml filename parameter for the wrapper framework to load otb application.
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT InputProcessXMLParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputProcessXMLParameter      Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputProcessXMLParameter, Parameter);

  itkGetStringMacro(FileName);

  bool HasValue() const override
  {
    if(m_FileName.empty())
      return false;
    else
      return true;
  }

  virtual ParameterType GetType() const override
  {
    return ParameterType_InputProcessXML;
  }

  std::string ToString() const override
  {
    return GetFileName();
  }

  void FromString(const std::string& value) override
  {
    SetFileName(value);
  }

  // Get Value
  //TODO otbGetObjectMemberMacro(StringParam, Value , std::string);

  bool SetFileName(std::string value);

  // Set Value
  virtual void SetValue(const std::string value);

  ImagePixelType GetPixelTypeFromString(std::string pixTypeAsString);

  ParameterType GetParameterType(const Parameter* param) const;

  const std::string GetChildNodeTextOf(TiXmlElement *parentElement, std::string key);

  int Read(Application::Pointer application);

  void otbAppLogInfo(Application::Pointer app, std::string info);

/* copied from Utilities/tinyXMLlib/tinyxml.cpp. Must have a FIX inside tinyxml.cpp */
  FILE* TiXmlFOpen( const char* filename, const char* mode );

protected:

  InputProcessXMLParameter();

  /** Destructor */
  ~InputProcessXMLParameter() override;

private:

  std::string    m_FileName;

  InputProcessXMLParameter(const InputProcessXMLParameter &) = delete;
  void operator =(const InputProcessXMLParameter&) = delete;


}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
