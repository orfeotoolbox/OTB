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

#ifndef otbWrapperOutputProcessXMLParameter_h
#define otbWrapperOutputProcessXMLParameter_h
#include "otbWrapperApplication.h"
#include "otb_tinyxml.h"
#include <string>

namespace otb
{
namespace Wrapper
{

/** \class OutputProcessXMLParameter
 *  \brief This class represent a xml filename parameter for the wrapper framework to save otb application.
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT OutputProcessXMLParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputProcessXMLParameter     Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputProcessXMLParameter, Parameter);

  itkGetStringMacro(FileName);

  // Set FileName
  void SetFileName(std::string value)
  {
    this->SetValue(value);
  }

  // Set Value
  virtual void SetValue(const std::string value)
    {
    itkDebugMacro("setting member m_FileName to " << value);
    this->m_FileName = value;
    SetActive(true);
    this->Modified();
    }

  bool HasValue() const override
  {
    if(m_FileName.empty())
      return false;
    else
      return true;
  }

  void ClearValue() override
  {
    m_FileName = "";
  }

  TiXmlElement* AddChildNodeTo(TiXmlElement* parent, std::string name, std::string value="");

  std::string pixelTypeToString(ImagePixelType pixType);

  ParameterType GetParameterType(const Parameter* param) const;

  void Write(Application::Pointer application);

  TiXmlElement* ParseApplication(Application::Pointer app);

protected:

  OutputProcessXMLParameter();

  /** Destructor */
  ~OutputProcessXMLParameter() override;

private:

  /** Recursive function to parse a group parameter */
  void ParseGroup(const std::string& group);

  std::string    m_FileName;

  /** Temporary storage of the XML node to fill (internal) */
  TiXmlElement* m_Node;

  /** Temporary pointer to the application to parse (internal) */
  Application::Pointer m_Appli;

  OutputProcessXMLParameter(const OutputProcessXMLParameter &) = delete;
  void operator =(const OutputProcessXMLParameter&) = delete;


}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
