/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperExpStringParameter_h
#define otbWrapperExpStringParameter_h

#include "otbWrapperExpSingleParameter.h"
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace WrapperExp
{

class OTBApplicationEngine_EXPORT StringParameter : public SingleParameter
{
public:
  typedef StringParameter Self;
  typedef SingleParameter Superclass;
  typedef std::shared_ptr<Self> Pointer;
  typedef std::shared_ptr<const Self> ConstPointer;

  itkTypeMacro( StringParameter , SingleParameter );

  static Pointer New()
    {
    return std::shared_ptr<Self>( new Self() );
    }

  virtual void SetDefaultValue( std::string val)
    {
    m_Default = std::move(val);
    }

  virtual const std::string & GetDefaultValue() const
    {
    return m_Default;
    }

  virtual void Reset() override 
    {
    m_Value = m_Default;
    }

  virtual ~StringParameter() override = default;

protected:
  StringParameter() = default;


private:

  std::string m_Default;

};

} // end namespace WrapperExp
} // end namespace otb

#endif
