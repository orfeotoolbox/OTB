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

#ifndef otbWrapperBoolParameter_h
#define otbWrapperBoolParameter_h

#include "otbWrapperParameter.h"
#include "OTBApplicationEngineExport.h"
#include <string>

namespace otb
{
namespace Wrapper
{

/** \class BoolParameter
 *  \brief This class represent a boolean parameter for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT BoolParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef BoolParameter                 Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(BoolParameter, Parameter);

  /** This parameter is ON/OFF switch, it always has a value */
  bool HasValue() const override
  {
    return true;
  }

  bool        GetValue() const;
  std::string GetValueAsString() const;

  void SetValue(bool state);
  void SetValue(const std::string& str);

  ParameterType GetType() const override;

  int         ToInt() const override;
  std::string ToString() const override;

  void FromString(const std::string& value) override;
  void FromInt(int value) override;
  ;

protected:
  /** Constructor */
  BoolParameter();

  /** Destructor */
  ~BoolParameter() override
  {
  }

private:
  BoolParameter(const BoolParameter&) = delete;
  void operator=(const BoolParameter&) = delete;

  bool m_Value;
};

} // end of namespace Wrapper
} // end of namespace otb

#endif
