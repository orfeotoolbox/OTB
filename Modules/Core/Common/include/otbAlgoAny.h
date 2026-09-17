/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbAlgoAny_h
#define otbAlgoAny_h

#include <algorithm>
#include <initializer_list>
#include <type_traits>

namespace otb
{

/**
 * Functor that converts its input into `bool`.
 * \pre The input type needs to be convertible to `bool`s.
 */
struct as_bool
{
  template <typename T, class = std::enable_if<std::is_nothrow_constructible<T, bool>::value>>
  constexpr bool operator()(T const& o) const noexcept
  {
    return o;
  }
};

/**
 * Port of `std::all_of` to `std::initializer_list`.
 * \tparam T  Types of the elements to analyse
 * \tparam P  Type of predicate that applies on `T` values.
 * \param[in] list  initializer list of elements to analyse
 * \param[in] p     actual predicate
 *
 * \return whether **all** elements from `list` evaluate to `true` through the predicate
 * \throw None
 */
template <typename T, typename P = as_bool>
bool all_of(std::initializer_list<T> const& list, P p = P{})
{
  return std::all_of(std::begin(list), std::end(list), p);
}

/**
 * Port of `std::any_of` to `std::initializer_list`.
 * \tparam T  Types of the elements to analyse
 * \tparam P  Type of predicate that applies on `T` values.
 * \param[in] list  initializer list of elements to analyse
 * \param[in] p     actual predicate
 *
 * \return whether **any** element from `list` evaluates to `true` through the predicate
 * \throw None
 */
template <typename T, typename P = as_bool>
bool any_of(std::initializer_list<T> const& list, P p = P{})
{
  return std::any_of(std::begin(list), std::end(list), p);
}

/**
 * Port of `std::none_of` to `std::initializer_list`.
 * \tparam T  Types of the elements to analyse
 * \tparam P  Type of predicate that applies on `T` values.
 * \param[in] list  initializer list of elements to analyse
 * \param[in] p     actual predicate
 *
 * \return whether **no** element from `list` evaluates to `true` through the predicate
 * \throw None
 */
template <typename T, typename P = as_bool>
bool none_of(std::initializer_list<T> const& list, P p = P{})
{
  return std::none_of(std::begin(list), std::end(list), p);
}

} // otb namespace


#endif // otbAlgoAny_h
