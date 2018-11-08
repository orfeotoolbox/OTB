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

#ifndef otb_VariadicConcatenateFunctor_h
#define otb_VariadicConcatenateFunctor_h

#include <vector>
#include <numeric>

namespace otb
{

namespace Functor
{


namespace internal
{
// helper function to implement next functor (convert a scalar value
// to a VariableLengthVector)
template <typename T> itk::VariableLengthVector<T> toVector(const T & in)
{
  itk::VariableLengthVector<T> out;
  out.SetSize(1);
  out[0] = in;
  return out;
}

// helper function to implement next functor, VariableLengthVectorVersion (returns in)
template <typename  T> const itk::VariableLengthVector<T> & toVector(const itk::VariableLengthVector<T> & in)
{
  return in;
}

// helper function to implement next functor, Merge two VariableLengthVector in-place
template <typename v1, typename v2> void concatenateVectors(v1 & a, const v2 & b)
{
  const size_t previousSizeOfA = a.GetSize();
  
  a.SetSize(previousSizeOfA+b.GetSize());
  
  for(size_t it = 0; it<b.Size();++it)
    {
    a[previousSizeOfA+it] = static_cast<typename v1::ValueType>(b[it]);
    }
}

// helper function to implement next functor, Merge N VariableLengthVector in-place
template <typename v1, typename v2, typename ...vn> void concatenateVectors(v1 & a, const v2 & b, const vn&... z)
{
  concatenateVectors(a,b);
  concatenateVectors(a,z...);
}
} // end namespace internal

// N  images (all types) -> vector image
// This functor concatenates N images (N = variadic) of type
// VectorImage and or Image, into a single VectorImage
template<typename TOut, typename ...TIns> struct VariadicConcatenate
{
  auto operator()(const TIns &...  ins) const
  {
    itk::VariableLengthVector<TOut> out;
    internal::concatenateVectors(out, internal::toVector(ins)...);
    
    return out;
  }

  // Must define OutputSize because output pixel is vector
  constexpr size_t OutputSize(const std::array<size_t, sizeof...(TIns)> inputsNbBands) const
  {
    return std::accumulate(inputsNbBands.begin(),inputsNbBands.end(),0);
  }
};

} // end namespace Functor

} // end namespace otb

#endif
