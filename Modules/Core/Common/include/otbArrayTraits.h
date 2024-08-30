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

#ifndef otbArrayTraits_h
#define otbArrayTraits_h

#include "itkDefaultConvertPixelTraits.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"

namespace otb
{
namespace mpl
{

/*===============================[ otb::is_array<> ]=========================*/
namespace internals
{

/** SFINAE workaround to match types that inherit from `itk::FixedArray`.
 * As we cannot use `std::is_base_of<RGBAPixel<T>, itk::FixedArray<T, N>`
 * when trying to see if `RGBAPixel` inherits from `itk::FixedArray`
 * without knowing the `N` parameter beforehand, here is this
 * workaround.
 *
 * We take the parameter by pointer to avoid possible conversions.
 * \ingroup OTBCommonInternals
 */
template <typename T, unsigned int N>
constexpr std::true_type is_array_f(itk::FixedArray<T, N> const*);

/** SFINAE part that says: "ignore anything else".
 * \ingroup OTBCommonInternals
 */
template <typename... T>
constexpr std::false_type is_array_f(...);

static_assert(decltype(internals::is_array_f(std::declval<itk::FixedArray<int, 1>*>()))::value, "it::FixedArray should be matched");
} // internals namespace

/**
 * Type traits to identify any array pixel types.
 * \tparam T  Type to check
 *
 * This generic flavour recognizes types that inherit from
 * `itk::FixedArray`, and says "no" to any thing else.
 *
 * At this moment, there is no specialization for `std::vector` nor types
 * that inherit from `vnl_vector`.
 *
 * \ingroup OTBCommon
 * \sa `is_array_v<>`
 *
 * \internal it could be extended/specialized either through
 * `internals::is_array_f()` or by specializing this type (when
 * possible).
 */
template <typename T>
struct is_array
: decltype(internals::is_array_f(std::declval<T*>()))
{};

/** `is_array<>` specialization for recognizing `itk::VariableLengthVector<>`.
 * \ingroup OTBCommon
 */
template <typename T>
struct is_array<itk::VariableLengthVector<T>>
: std::true_type
{};

/** `is_array<>` specialization for recognizing expression templates
 * based on `itk::VariableLengthVector<>`.
 * \ingroup OTBCommon
 */
template <typename TExpr1, typename TExpr2, typename TBinaryOp>
struct is_array<itk::VariableLengthVectorExpression<TExpr1, TExpr2,TBinaryOp>>
: std::true_type
{};


/**
 * Helper variable to identify any array pixel types.
 * \tparam T  Type to check
 *
 * \return `true` for types that inherit from `itk::FixedArray`
 * \return `true` for `itk::VariableLengthVector`, and expression
 * templates made of VLV.
 * \return `false` otherwise
 *
 * At this moment, there is no specialization for `std::vector` nor types
 * that inherit from `vnl_vector`.
 *
 * \ingroup OTBCommon
 * \sa `is_array<>`
 */
template <typename T>
constexpr bool is_array_v = is_array<T>::value;

// static_assert(decltype(internals::is_array_f(std::declval<itk::RGBAPixel<int>*>()))::value, "");

// static_assert(is_array_v<itk::RGBAPixel<double>>, "");
static_assert(is_array_v<itk::FixedArray<double, 42>>, "");
static_assert(is_array_v<itk::FixedArray<std::complex<double>, 42>>, "");
static_assert(is_array_v<itk::VariableLengthVector<double>>, "");
static_assert(is_array_v<itk::VariableLengthVector<std::complex<double>>>, "");

/*===============================[ GetNumberOfComponents ]===================*/
namespace internals
{

/**
 * Traits for accessing the number of components associated to a pixel.
 * \tparam PixelType  automatically deduced.
 *
 * \internal While there is `itk::DefaultConvertPixelTraits` and
 * `otb::DefaultConvertPixelTraits`, they don't handle correctly
 * - `std::complex<>`, as there is no
 * `itk::DefaultConvertPixelTraits<complex<>>::GetNumberOfComponents(pix)`
 * - nor `itk::VariableLengthVector<>`, as it copies the VLV, which is
 * an expensive operation.
 *
 * \sa `otb::mpl::GetNumberOfComponents()`
 * \ingroup OTBCommonInternals
 * \todo it should have been `constexpr` and `noexcept`, but the ITK
 * flavours we rely upon aren't...
 */
template <typename PixelType>
struct NumberOfComponents
{
  static unsigned int For(PixelType const& pix)
  {
    return itk::DefaultConvertPixelTraits<std::remove_const_t<PixelType>>::GetNumberOfComponents(pix);
  }
};

/** Specialization for `itk::VariableLengthVector<>`
 * \ingroup OTBCommonInternals
 */
template <typename RealType>
struct NumberOfComponents<itk::VariableLengthVector<RealType>>
{
  // ITK definition of
  // DefaultConvertPixelTraits<VLV>::GetNumberOfComponents() takes the
  // parameter by value, and thus create a new VLV by copy.
  // This is too expensive! Let's work around it.
  static unsigned int For(itk::VariableLengthVector<RealType> const& pix)
  {
    return pix.GetSize();
  }
};

/** Specialization for `std::complex<>`
 * \ingroup OTBCommonInternals
 */
template <typename T>
struct NumberOfComponents<std::complex<T>>
{
  static constexpr unsigned int For(std::complex<T> const&) noexcept
  {
    return 2;
  }
};

/** Specialization for `const` types.
 * This specialization automatically removes the `const` qualifier.
 * \ingroup OTBCommonInternals
 */
template <typename T>
struct NumberOfComponents<T const>
{
  static unsigned int For(T const& pix)
  {
    return NumberOfComponents<T>::For(pix);
  }
};
} // internals namespace

/**
 * Returns the number of components in the pixel parameter.
 * While `itk::DefaultConvertPixelTraits` and
 * `otb::DefaultConvertPixelTraits` already exist, they don't handle
 * correctly:
 * - `std::complex<>`, as there is no
 * `itk::DefaultConvertPixelTraits<complex<>>::GetNumberOfComponents(pix)`
 * - nor `itk::VariableLengthVector<>`, as it copies the VLV, which is
 * an expensive operation.
 *
 * \tparam PixelType  automatically deduced pixel type
 * \param[in] pix     pixel parameter
 * \return The number of components in the pixel.
 * \throw None
 */
template <typename PixelType>
inline
unsigned int GetNumberOfComponents(PixelType const& pix)
{
  return internals::NumberOfComponents<PixelType>::For(pix);
}

} // otb::mpl namespace
} // otb namespace

#endif  // otbArrayTraits_h
