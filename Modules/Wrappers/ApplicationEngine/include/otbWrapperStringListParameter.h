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

#ifndef otbWrapperStringListParameter_h
#define otbWrapperStringListParameter_h


#include "otbWrapperParameterList.h"
#include "otbWrapperStringParameter.h"


namespace otb
{


namespace Wrapper
{

/** \class StringListParameter
 *  \brief This class represent a list of string parameter for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT StringListParameter : public ParameterList<StringParameter>
{
  //
  // Public methods.
public:
  /** Standard class typedef */
  typedef StringListParameter            Self;
  typedef ParameterList<StringParameter> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  typedef StringListInterface::StringVector StringListType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(StringListParameter, ParameterList);

  /** Set the value */
  void SetValue(const StringListInterface::StringVector&);

  /** */
  void AddString(const std::string& value);

  /** Get the value */
  StringListInterface::StringVector GetValue() const;

  /** Get the value */
  std::string GetNthElement(std::size_t) const;

  /** Get the value */
  void SetNthElement(std::size_t, const std::string&);

  /** */
  using StringListInterface::GetDirection;
  Role GetDirection() const override;

  /** */
  bool IsFilename() const override;

  ParameterType GetType() const override
  {
    return ParameterType_StringList;
  }

  //
  // Protected methods.
protected:
  /** Constructor */
  StringListParameter();

  /** Destructor */
  ~StringListParameter() override;

  //
  // Private methods.
private:
  StringListParameter(const StringListParameter&) = delete;

  void operator=(const StringListParameter&) = delete;

}; // End class Parameter

} // End namespace Wrapper

} // End namespace otb

#endif
