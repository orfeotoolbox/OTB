/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbNotNull_h
#define otbNotNull_h

#include <cassert>

namespace otb
{
/**
 * Simplified version of `GSL::not_null`.
 *
 * Permits to holds pointers that shall never be null.
 *
 * `GSL::not_null` is under MIT licence
 */
template <typename T>
class NotNull
{
public:
  constexpr NotNull(T p) : m_p(p) { assert(p); }
  NotNull           (std::nullptr_t) = delete;
  NotNull& operator=(std::nullptr_t) = delete;

  constexpr T get() const
  {
    assert(m_p);
    return m_p;
  }

  constexpr                operator T() const { return get(); }
  constexpr T              operator->() const { return get(); }
  constexpr decltype(auto) operator*()  const { return *get(); }

  // unwanted operators...pointers only point to single objects!
  NotNull& operator++()                     = delete;
  NotNull& operator--()                     = delete;
  NotNull  operator++(int)                  = delete;
  NotNull  operator--(int)                  = delete;
  NotNull& operator+=(std::ptrdiff_t)       = delete;
  NotNull& operator-=(std::ptrdiff_t)       = delete;
  void     operator[](std::ptrdiff_t) const = delete;

private:
  T m_p;
};

} // otb namespace

#endif  // otbNotNull_h
