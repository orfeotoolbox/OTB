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

#include "otbMetaProgrammingLibrary.h"
#include <string>
#include <vector>
#include <array>

using namespace otb;

static_assert(is_best_returned_by_value_v<int>,        "trivial type is returned by value");
static_assert(is_best_returned_by_value_v<double>,     "trivial type is returned by value");
static_assert(! is_best_returned_by_value_v<double &>, "reference is returned by const ref");

static_assert(! is_best_returned_by_value_v<std::vector<int>>, "vector is returned by const ref");
static_assert(! is_best_returned_by_value_v<std::string>,      "string is returned by const ref");

static_assert(  is_best_returned_by_value_v<std::array<double, 2>>, "small array is returned by value");
static_assert(! is_best_returned_by_value_v<std::array<double, 3>>, "big array is returned by const ref");
static_assert(  is_best_returned_by_value_v<std::array<float, 4>>,  "small array is returned by value");

template <typename T> struct V2d {
  T x;
  T y;
};
template <typename T> struct V3d {
  T x;
  T y;
  T z;
};

static_assert(  is_best_returned_by_value_v<V2d<double>>, "small POD is returned by value");
static_assert(! is_best_returned_by_value_v<V3d<double>>, "big POD is returned by const ref");
static_assert(  is_best_returned_by_value_v<V3d<float>>,  "small POD is returned by value");
