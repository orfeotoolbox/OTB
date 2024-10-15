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

#ifndef otbAlgoClamp_h
#define otbAlgoClamp_h

namespace otb
{
/**
 * Backport of C++17 `std::clamp` to C++14.
 * Clamps the value `v` to [`lo`, `hi`]  interval.
 * @throw None
 */
template <typename T>
constexpr
T const& clamp(T const& v, T const& lo, T const& hi) noexcept
{
  assert( lo <= hi );
  return (v < lo) ? lo
    : (hi < v) ? hi
    :            v;
}
} // otb namespace

#endif  // otbAlgoClamp_h
