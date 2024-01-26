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

#ifndef OtbNumericTraits_h
#define OtbNumericTraits_h

#include <limits>
#include <cmath>
#include <type_traits>
#include <algorithm>

namespace otb
{

template <typename I, typename O>
constexpr auto common_lowest() {
    using C = std::common_type_t<I,O>;
    return
      std::is_unsigned<I>::value ? I{}
    : std::is_unsigned<O>::value ? O{}
    : std::max<C>(
            static_cast<C>(std::numeric_limits<I>::lowest()),
            static_cast<C>(std::numeric_limits<O>::lowest())
            );
}

template <typename I, typename O>
constexpr auto common_highest() {
    using C = std::common_type_t<I,O>;
    return std::min<C>(
            static_cast<C>(std::numeric_limits<I>::max()),
            static_cast<C>(std::numeric_limits<O>::max()));
}

} // Namespace otb

#endif // OtbNumericTraits_h
