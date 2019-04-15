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

#ifndef otbIndicesStackFunctor_h
#define otbIndicesStackFunctor_h

#include <vector>
#include <stdexcept>

namespace otb {

namespace Functor {
/**
 * \class IndicesStackFunctor
 * \brief A class to compute a stack of radiometric indices
 * 
 * This functor can be built from a vector of TIndice*. its operator()
 * will apply each functor of this vector to the input pixel, and
 * return a VariableLengthVector containing the list resulting
 * values. It can be used with otb::FunctorImageFilter
 * \sa FunctorImageFilter
 * \ingroup Indices
 */
template <typename TIndice>
class IndicesStackFunctor
{
public:
  /// Read input / output types from TIndice
  using IndiceType = TIndice;
  using PixelType  = typename IndiceType::PixelType;
  // Output will be a VariableLengthVector of values return by
  // radiometric indices
  using OutputType = itk::VariableLengthVector<typename IndiceType::OutputType>;

  /**
   * \param indices A std::vector<IndiceType*> for indices to compute
   * the indice stack
   * \throw std::runtime_error if indices is empty
   */
  IndicesStackFunctor(const std::vector<IndiceType*> & indices) 
    : m_Indices(indices)
  {
    if(indices.empty())
      {
      throw std::runtime_error("Can not build IndicesStackFunctor from an empty list of indices.");
      }
  }

  /** 
   * \param input A itk::VariableLengthVector<TInput> holding the
   * pixel values for each band
   * \return A VariableLengthVector<TInput::OutputType> holding all
   * the indices values
   */
  void operator()(OutputType & out, const PixelType & in) const
  {
    size_t idx = 0;
    for(auto indice : m_Indices)
      {
      out[idx] = (*indice)(in);
      ++idx;
      }
  }
  /**
   * \return the size of the indices list (to be used by FunctorImgeFilter) 
   */
  size_t OutputSize(...) const
  {
    return m_Indices.size();
  } 

private:
  /// The list of indices to use
  std::vector<IndiceType*> m_Indices;
};

} // End namespace Functor

} // End namespace otb

#endif
