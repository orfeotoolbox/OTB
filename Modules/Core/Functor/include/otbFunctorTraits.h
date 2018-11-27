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

#ifndef otbFunctorTraits_h
#define otbFunctorTraits_h

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "itkFixedArray.h"


namespace otb {

/**  
 * \struct IsNeighborhood 
 * \brief Struct testing if T is a neighborhood
 * 
 * Provides:
 * - ValueType type set to false_type or true_type
 * - value set to true or false
 */
template <class T> struct IsNeighborhood : std::false_type {};

/// Partial specialisation for itk::Neighborhood<T>
template <class T> struct IsNeighborhood<itk::Neighborhood<T>> : std::true_type {};


/// Partial specialisation for const itk::Neighborhood<T> &
template <class T> struct IsNeighborhood<const itk::Neighborhood<T>&> : std::true_type {};

/**
 * \struct IsSuitableType
 * \brief Helper struct to check if a type can be used as pixel type.
 *
 * ::value maps to true if type can be used and false otherwhise.
 */
template <class T> struct IsSuitableType : std::is_scalar<T>::type {};

/// Unwrap complex
template <class T> struct IsSuitableType<std::complex<T>> : IsSuitableType<T>::type {};

/// Unwrap VariableLengthVector
template <class T> struct IsSuitableType<itk::VariableLengthVector<T>> : IsSuitableType<T>::type {};

/// Unwrap FixedArray
template <class T, unsigned int N> struct IsSuitableType<itk::FixedArray<T,N>> : IsSuitableType<T>::type {};

/// Unwrap RGBPixel
template <class T> struct IsSuitableType<itk::RGBPixel<T>> : IsSuitableType<T>::type {};

/// Unwrap RGBAPixel
template <class T> struct IsSuitableType<itk::RGBAPixel<T>> : IsSuitableType<T>::type {};

/**
 * \struct PixelTypeDeduction
 * \brief Helper struct to derive PixelType from template parameter.
 * 
 * T                           -> PixelType = T
 * itk::Neighborhood<T>        -> PixelType = T
 * const itk::Neighborhood<T>& -> PixelType = T
*/
template <class T> struct PixelTypeDeduction
{
  static_assert(IsSuitableType<T>::value,
                "T can not be used as a template parameter for Image or VectorImage classes.");
  using PixelType = T;
};

/// Partial specialisation for itk::Neighborhood<T>
template <class T> struct PixelTypeDeduction<itk::Neighborhood<T>>
{
  static_assert(IsSuitableType<T>::value,
                "T can not be used as a template parameter for Image or VectorImage classes.");
  using PixelType = T;
};

/** 
 * \struct ImageTypeDeduction
 * \brief Helper struct to derive ImageType from template parameter
 * 
 * T                            -> ImageType = otb::Image<T>
 * itk::VariableLengthVector<T> -> ImageType = otb::VectorImage<T>
 * const T &                    -> ImageType = ImageTypeDeduction<T>::ImageType
 */
template <class T> struct ImageTypeDeduction 
{
  using ImageType = otb::Image<T>;
};

/// Partial specialisation for itk::VariableLengthVector<T>
template <class T> struct ImageTypeDeduction<itk::VariableLengthVector<T>>
{
  using ImageType = otb::VectorImage<T>;
};

// Helper to remove const, volatite and Ref qualifier (until c++20
/// that has std::remove_cvref)
template <typename T> using RemoveCVRef = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

/**
* \struct RetrieveOperator
*
* \brief Struct to retrieve the operator type
* 
* \tparam T the type to retrieve operator() from
*
*/
template <typename T> struct RetrieveOperator
{
  static_assert(std::is_class<T>::value || std::is_function<T>::value, "T is not a class or function");
  using Type = decltype(&T::operator());
};

} // end namespace otb



#endif
