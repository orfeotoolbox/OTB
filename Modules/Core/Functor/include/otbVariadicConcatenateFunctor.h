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

#ifndef otb_VariadicConcatenateFunctor_h
#define otb_VariadicConcatenateFunctor_h

#include "itkVariableLengthVector.h"
#include <vector>
#include <numeric>
#include <array>

namespace otb
{

namespace Functor
{


namespace variadic_concatenate_details
{

template <typename T>
size_t NumberOfElements(const T&)
{
  static_assert(std::is_scalar<T>::value,
                "variadic_concatenate_details::NumberOfElements<T> only works for T and itk::VariableLengthVector<T> where T is a scalar type.");
  return 1;
}

template <typename T>
size_t NumberOfElements(const itk::VariableLengthVector<T>& v)
{
  static_assert(std::is_scalar<T>::value,
                "variadic_concatenate_details::NumberOfElements<T> only works for T and itk::VariableLengthVector<T> where T is a scalar type.");
  return v.GetSize();
}

template <typename... T>
size_t NumberOfElements(const T&... t)
{
  std::array<size_t, sizeof...(T)> sizes = {{NumberOfElements(t)...}};
  return std::accumulate(sizes.begin(), sizes.end(), 0);
}

template <typename Out, typename T>
size_t fillVector(itk::VariableLengthVector<Out>& out, size_t idx, const T& t)
{
  assert(idx < out.GetSize());
  out[idx] = static_cast<Out>(t);
  return idx + 1;
}

template <typename Out, typename T>
size_t fillVector(itk::VariableLengthVector<Out>& out, size_t idx, const itk::VariableLengthVector<T>& t)
{
  assert(idx + t.GetSize() <= out.GetSize());
  for (auto it    = 0UL; it < t.GetSize(); ++it)
    out[idx + it] = static_cast<Out>(t[it]);
  return idx + t.GetSize();
}

template <typename Out, typename Current, typename... T>
size_t fillVector(itk::VariableLengthVector<Out>& out, size_t idx, const Current& current, const T&... t)
{
  size_t newIdx = fillVector(out, idx, current);
  return fillVector(out, newIdx, t...);
}
} // end namespace variadic_concatenate_details

// N  images (all types) -> vector image
// This functor concatenates N images (N = variadic) of type
// VectorImage and or Image, into a single VectorImage

/**
 * \class VariadicConcatenate
 * \brief This functor concatenates any number of input of scalar type
 * or VariableLengthVector.
 * \ingroup OTBFunctor
 */
template <typename TOut, typename... TIns>
struct VariadicConcatenate
{
  auto operator()(const TIns&... ins) const
  {
    const size_t                    numberOfElements = variadic_concatenate_details::NumberOfElements(ins...);
    itk::VariableLengthVector<TOut> out(numberOfElements);

    variadic_concatenate_details::fillVector(out, 0, ins...);

    return out;
  }

  // Must define OutputSize because output pixel is vector
  constexpr size_t OutputSize(const std::array<size_t, sizeof...(TIns)> inputsNbBands) const
  {
    return std::accumulate(inputsNbBands.begin(), inputsNbBands.end(), 0);
  }
};

} // end namespace Functor

} // end namespace otb

#endif
