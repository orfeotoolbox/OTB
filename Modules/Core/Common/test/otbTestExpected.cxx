/*
 * Copyright (C) 2005-2025 Centre National d'Etudes Spatiales (CNES)
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

#include "otbExpected.h"

enum class Error {
  Unknown,
  NotFound,
  Impossible,
  DontMatch
};

constexpr auto un = otb::unexpected<Error>{};
static_assert(un.error() == Error::Unknown);

// ----[ Default construction
constexpr auto def = otb::expected<int, Error>{};
static_assert(def.has_value());
static_assert(def);
static_assert(def.value() == 0);
static_assert(*def == 0);
// static_assert(def.error() == Error::Unknown); // Fails to compile. OK!

// ----[ Init construction from value
constexpr auto one = otb::expected<int, Error>{1};
static_assert(one.has_value());
static_assert(one);
static_assert(one.value() == 1);
static_assert(*one == 1);
// static_assert(one.error() == Error::Unknown); // Fails to compile. OK!

// ----[ Init construction from unexpected
constexpr auto err_match = otb::expected<int, Error>{un};
static_assert(! err_match.has_value());
static_assert(! err_match);
// static_assert(err_match.value() == 1); // Fails to compile. OK!
static_assert(err_match.error() == Error::Unknown);

// ----[ From a function
constexpr otb::expected<int, Error> func(int i) {
  switch (i) {
    case 1: {
              auto res = otb::unexpected{Error::DontMatch};
              static_assert(otb::details::is_unexpected<std::remove_cvref_t<decltype((res))>>);
              return res;
            }
    case 2: return otb::unexpected{Error::Impossible};
    default: return {i};
  }
}

constexpr auto f1 = func(0);
static_assert(f1.has_value());
static_assert(*f1 == 0);

constexpr auto f2 = func(1);
static_assert(! f2.has_value());
static_assert(f2.error() == Error::DontMatch);

constexpr auto f3 = func(2);
static_assert(! f3.has_value());
static_assert(f3.error() == Error::Impossible);

constexpr auto f4 = func(3);
static_assert(f4.has_value());
static_assert(*f4 == 3);
