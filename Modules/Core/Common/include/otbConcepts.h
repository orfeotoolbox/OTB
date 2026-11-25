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

#ifndef otbConcepts_h
#define otbConcepts_h

#include <ranges>

namespace otb
{

/**
 * Concept for read only associative containers.
 */
template <typename T>
concept ReadOnlyAssociativeContainer
= std::ranges::forward_range<T>
&& requires(T a, const T ca, typename T::key_type k) {
  // Required nested types
  typename T::key_type;
  typename T::value_type;
  typename T::size_type;

  // Key-based lookup operations
#if 0
  { a.begin() } -> std::same_as<typename T::iterator>;
  { ca.begin() } -> std::same_as<typename T::const_iterator>;
  { a.cbegin() } -> std::same_as<typename T::const_iterator>;
  { ca.cbegin() } -> std::same_as<typename T::const_iterator>;

  { a.end() } -> std::same_as<typename T::iterator>;
  { ca.end() } -> std::same_as<typename T::const_iterator>;
  { a.cend() } -> std::same_as<typename T::const_iterator>;
  { ca.cend() } -> std::same_as<typename T::const_iterator>;
#endif

  { a.size() } -> std::same_as<typename T::size_type>;
  { ca.size() } -> std::same_as<typename T::size_type>;
  { a.empty() } -> std::same_as<bool>;
  { ca.empty() } -> std::same_as<bool>;

  { a.find(k) } -> std::same_as<typename T::iterator>;
  { ca.find(k) } -> std::same_as<typename T::const_iterator>;
  { ca.contains(k) } -> std::convertible_to<bool>; // C++20+
  { ca.count(k) } -> std::same_as<typename T::size_type>;
};
}

#endif // otbConcepts_h
