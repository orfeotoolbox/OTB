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


#ifndef otbMachineLearningModelTraits_h
#define otbMachineLearningModelTraits_h


#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"
#include "itkIsNumber.h"
#include "itkMetaProgrammingLibrary.h"

namespace otb
{

/**
 * This is the struct defining the sample implementation for
 * MachineLearningModel. It offers two type definitions: SampleType
 * and ValueType.
 *
 * \tparam TInput : input sample type (can be either a scalar type or
 * a VariableLengthVector
 * \tparam isNumber either TrueType or FalseType for partial
 * specialization

 */
template <typename TInput, typename isNumber>
struct MLMSampleTraitsImpl;


/// \cond SPECIALIZATION_IMPLEMENTATION
// For Numbers
template <typename TInput>
struct MLMSampleTraitsImpl<TInput, itk::mpl::TrueType>
{
  typedef TInput                            ValueType;
  typedef itk::VariableLengthVector<TInput> SampleType;
};

// For Vectors
template <typename TInput>
struct MLMSampleTraitsImpl<TInput, itk::mpl::FalseType>
{
  typedef typename TInput::ValueType ValueType;
  typedef TInput                     SampleType;
};
/// \endcond

/**
 * Simplified implementation of SampleTraits using MLMSampleTraitsImpl
 */
template <typename TInput>
using MLMSampleTraits = MLMSampleTraitsImpl<TInput, typename itk::mpl::IsNumber<TInput>::Type>;


/**
 * This is the struct defining the sample implementation for
 * MachineLearningModel. It offers two type definitions: TargetType
 * and ValueType.
 *
 * \tparam TInput : input sample type (can be either a scalar type or
 * a VariableLengthVector or a FixedArray
 * \tparam isNumber either TrueType or FalseType for partial
 * specialization

 */
template <typename TInput, typename isNumber>
struct MLMTargetTraitsImpl;


/// \cond SPECIALIZATION_IMPLEMENTATION
// For Numbers
template <typename TInput>
struct MLMTargetTraitsImpl<TInput, itk::mpl::TrueType>
{
  typedef TInput ValueType;
  typedef itk::FixedArray<TInput, 1> SampleType;
};

// For Vectors
template <typename TInput>
struct MLMTargetTraitsImpl<TInput, itk::mpl::FalseType>
{
  typedef typename TInput::ValueType ValueType;
  typedef TInput                     SampleType;
};
/// \endcond

/**
 * Simplified implementation of TargetTraits using MLMTargetTraitsImpl
 */
template <typename TInput>
using MLMTargetTraits = MLMTargetTraitsImpl<TInput, typename itk::mpl::IsNumber<TInput>::Type>;


} // End namespace otb

#endif
