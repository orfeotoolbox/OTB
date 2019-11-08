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

#ifndef otb_VariadicAddFunctor_h
#define otb_VariadicAddFunctor_h

#include <numeric>
#include <array>

namespace otb
{

namespace Functor
{
/**
 * \class VariadicAdd
 * \brief This functor adds any number of compile time scalar inputs
 * \ingroup OTBFunctor
 */
template <typename TOut, typename... TIns>
struct VariadicAdd
{
  auto operator()(TIns... ins) const
  {
    std::array<TOut, sizeof...(TIns)> outArray = {{static_cast<TOut>(ins)...}};

    return std::accumulate(outArray.begin(), outArray.end(), TOut{});
  }
};

} // end namespace Functor

} // end namespace otb

#endif
