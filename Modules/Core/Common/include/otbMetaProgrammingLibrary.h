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
#ifndef otbMetaProgrammingLibrary_h
#define otbMetaProgrammingLibrary_h

#include <climits>
#include <tuple>
#include <type_traits>

namespace otb
{
/** `std::void_t` is defined in C++17.
 * \todo deprecate it once OTB drop C++14 compatibility.
 */
template <typename...> using void_t = void;

/// Returns the first type from a parameter pack
template <typename ... Ts>
using FrontType = typename std::tuple_element<0, std::tuple<Ts...>>::type;
// or Ts...[0] ; // in C++26...

/// Backport of C++20 `std::remove_cvref_t`
template <typename T>
using remove_cvref_t = typename std::remove_reference<typename std::remove_cv<T>::type>::type;


/** Trait that tells whether a type is best returned by value.
 * @return `std::true_type` iff `std::is_trivially_copyable_v<T>` and type < 2*64 bits
 * \note 8 correspond to an expected CHAR_BIT value of 8
 * \note Why 2*64 bits? Because Itanium ABI permits to dispatch uniform types that are trivially
 * copyable through registers when copying them in and out of functions.
 */
template <typename T>
struct is_best_returned_by_value
: std::integral_constant<bool, std::is_trivially_copyable<T>::value && (CHAR_BIT * sizeof(T) <= 128)>
{};

/** Tells whether a type is best returned by value.
 * @return whether `std::is_trivially_copyable_v<T>` and type < 2*64 bits
 */
template <typename T>
constexpr bool is_best_returned_by_value_v = is_best_returned_by_value<T>::value;

/**
 * Trait that tells the best way to return a `T`.
 * @return `T` if `is_best_returned_by_value_v<T>`
 * @return `T const&` otherwise
 */
template <typename T>
struct best_way_to_return
: std::conditional<is_best_returned_by_value_v<T>, T, T const&>
{};

/**
 * Helper alias trait that tells the best way to return a `T`.
 * @return `T` if `is_best_returned_by_value_v<T>`
 * @return `T const&` otherwise
 */
template <typename T>
using best_way_to_return_t = typename best_way_to_return<T>::type;

/**
 * Traits that tells whether a type is a tuple.
 */
template <typename T>
struct is_tuple : std::false_type {};

template <typename... U>
struct is_tuple<std::tuple <U...>> : std::true_type {};

/**
 * Tells whether a type is a tuple.
 */
template <typename T>
constexpr bool is_tuple_v = is_tuple<remove_cvref_t<T>>::value;

} // otb namespace


#endif // otbMetaProgrammingLibrary_h
