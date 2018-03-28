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

#ifndef otbWrapperExpNumericalParameter_h
#define otbWrapperExpNumericalParameter_h

#include <type_traits>
#include "otbWrapperExpNumericalParameterInterface.h"
#include "otbWrapperExpSingleParameter.h"
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace WrapperExp
{

template < typename T >
class OTBApplicationEngine_EXPORT NumericalParameter : 
public SingleParameter , public NumericalParameterInterface
{
public:
  typedef NumericalParameter           Self;
  // typedef SingleParameter              Superclass;
  typedef std::shared_ptr<Self>        Pointer;
  typedef std::shared_ptr<const Self>  ConstPointer;
  typedef T                            ValueType;

  // itkTypeMacro( NumericalParameter , SingleParameter );

  static Pointer New()
    {
    return std::shared_ptr<Self>( new Self() );
    }


  // override for NumericalParameterInterface inheritance
  virtual any_numeric GetAnyNumericValue() const override 
    {
    return any_numeric(m_NumValue);
    }

  virtual T GetNumericValue() const 
    {
    return m_NumValue;
    }


  virtual double GetFloattingPointValue() const override 
    {
    return GetNumericValue();
    }

  virtual int GetIntegerValue() const override
    {
    return GetNumericValue(); // might need to take care of this conversion
    }
// do we keep this
  virtual T GetDefaultTypeValue() const
    {
    return m_Boundaries[1].as<T>();
    }
// do we keep this
  virtual T GetMaximumTypeValue() const
    {
    return m_Boundaries[0].as<T>();
    }
// do we keep this
  virtual T GetMinimumTypeValue() const
    {
    return m_Boundaries[2].as<T>();
    }

  virtual ~NumericalParameter() override = default ;

protected:
  NumericalParameter();

  ValueType m_NumValue;

private:
};

} // end namespace WrapperExp
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWrapperExpNumericalParameter.txx"
#endif

#endif
