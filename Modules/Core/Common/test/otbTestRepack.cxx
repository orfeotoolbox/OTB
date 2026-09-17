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

#include "otbRepack.h"

#define BOOST_TEST_MODULE "otb::repack unit testing"
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "itkFixedArray.h"
#include "itkPoint.h"
#include "itkVariableLengthVector.h"
#include <random>
// #include <ostream>

namespace otb
{
namespace details
{
static_assert(is_size_known_type<itk::FixedArray<int, 3>>::value, "");
static_assert(is_size_known_type<std::array<int, 3>>::value, "");
static_assert(! is_size_known_type<itk::VariableLengthVector<int>>::value, "");

static_assert(std::is_same<double, common_underlying_t<double, double, double>>::value, "Double!");
static_assert(std::is_same<int, common_underlying_t<int, int, int>>::value, "Int!");

static_assert(std::is_same<double, common_underlying_t<itk::Point<double,2>, double>>::value, "Double!");
static_assert(std::is_same<int, common_underlying_t<itk::Point<int,2>, int>>::value, "Int!");

static_assert(std::is_same<void, common_array_t<int, double, double>>::value, "void");
static_assert(std::is_same<std::array<int,1>, typename common_array1<std::array<double, 1>>::type>::value, "array");
static_assert(std::is_same<std::array<int,1>, typename merge_common_array<std::array<int, 1>, void>::type>::value, "array");

static_assert(std::is_same<std::array<int,1>, common_array_t<std::array<double, 2>, double>>::value, "array");
static_assert(std::is_same<std::array<int,1>, common_array_t<double, std::array<double, 2>, double, double>>::value, "array");
static_assert(std::is_same<std::array<int,1>, common_array_t<std::array<double, 2>, double, double>>::value, "array");
static_assert(std::is_same<std::array<int,1>, common_array_t<std::array<double, 2>, double, std::array<double, 4>, double>>::value, "array");
static_assert(std::is_same<bool, common_array_t<double, std::array<double, 2>, itk::Point<double, 4>, double>>::value, "cannot be resolved automatically");
static_assert(std::is_same<itk::Point<double, 3>, common_array_g<double, 3, itk::Point<double,3>, double, std::array<double, 2>, itk::FixedArray<double, 4>, double>>::value, "cannot be resolved automatically");
} // otb::details
} // otb


namespace
{ // Anonymous namespace

template <typename T, bool is_integral>
struct uniform_distribution {};

template <typename T>
struct uniform_distribution<T, true> {
  using type = std::uniform_int_distribution<T>;
};

template <typename T>
struct uniform_distribution<T, false> {
  using type = std::uniform_real_distribution<T>;
};

template <typename T>
using uniform_distribution_t
= typename uniform_distribution<T, std::is_integral<T>::value>::type;


template <typename T>
class RNG
{
public:
  explicit RNG(T min_, T max_)
    : m_device()
    , m_engine(m_device())
    , m_distribution(min_, max_)
    {}

  explicit RNG()
      : RNG(std::numeric_limits<T>::min(), std::numeric_limits<T>::max())
      {}

  T generate() const { return m_distribution(m_engine); }

private:
  std::random_device               mutable m_device;
  std::default_random_engine       mutable m_engine;
  uniform_distribution_t<T>        mutable m_distribution;
};

} // Anonymous namespace


/*===============================[ Repacking x, y, z ==> tab[3] ]============*/

template <typename T, typename ArrayType>
void test_3numbers()
{
  using out_array_type = ArrayType;

  {
    auto pack = otb::repack<out_array_type>(1, 2, 3);
    BOOST_REQUIRE(otb::details::cumulated_size(pack) == 3);
    BOOST_CHECK(pack[0] == 1);
    BOOST_CHECK(pack[1] == 2);
    BOOST_CHECK(pack[2] == 3);
  }

  {
    RNG<T> rng;
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      auto pack = otb::repack<out_array_type>(x, y, z);
      BOOST_REQUIRE(otb::details::cumulated_size(pack) == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }
  }
}

template <typename T>
void auto_test_3numbers()
{
  {
    // auto pack = otb::auto_repack<itk::FixedArray>(1, 2, 3);
    auto pack = otb::auto_repack(T(1), 2, 3);
    static_assert(std::is_same<decltype(pack), itk::FixedArray<T,3>>::value, "auto -> FixedArray!");
    BOOST_REQUIRE(pack.Size() == 3);
    BOOST_CHECK(pack[0] == 1);
    BOOST_CHECK(pack[1] == 2);
    BOOST_CHECK(pack[2] == 3);
  }

#if 1
  {
    auto pack = otb::auto_repack<itk::Point>(T(1), 2, 3);
    static_assert(std::is_same<decltype(pack), itk::Point<T,3>>::value, "itk::Point expected");
    BOOST_REQUIRE(pack.Size() == 3);
    BOOST_CHECK(pack[0] == 1);
    BOOST_CHECK(pack[1] == 2);
    BOOST_CHECK(pack[2] == 3);
  }
#endif

  {
    auto pack = otb::auto_repack(1, 2, 3);
    BOOST_REQUIRE(pack.Size() == 3);
    BOOST_CHECK(pack[0] == 1);
    BOOST_CHECK(pack[1] == 2);
    BOOST_CHECK(pack[2] == 3);
  }

  {
    RNG<T> rng;
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      auto pack = otb::auto_repack<itk::FixedArray>(x, y, z);
      static_assert(std::is_same<decltype(pack), itk::FixedArray<T,3>>::value, "auto -> FixedArray!");
      BOOST_REQUIRE(pack.Size() == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }
  }
}

BOOST_AUTO_TEST_CASE(Pack3Ints)
{
  test_3numbers<int, itk::FixedArray<int, 3>>();
  test_3numbers<int, std::array<int, 3>>();
  test_3numbers<int, itk::VariableLengthVector<int>>();
  auto_test_3numbers<int>();
}

BOOST_AUTO_TEST_CASE(Pack3Doubles)
{
  test_3numbers<double, itk::FixedArray<double, 3>>();
  test_3numbers<double, itk::Point<double, 3>>();
  test_3numbers<double, std::array<double, 3>>();
  test_3numbers<double, itk::VariableLengthVector<double>>();
  auto_test_3numbers<double>();
}

/*===============================[ Repacking tab[2], z ==> tab[3] ]==========*/
template <typename T>
void test_tab2_plus_z()
{
  using in_array_type  = itk::FixedArray<T, 2>;
  using out_array_type = itk::FixedArray<T, 3>;

  {
    in_array_type in;
    in[0] = 1;
    in[1] = 2;
    auto pack = otb::repack<out_array_type>(in, 3);
    static_assert(std::is_same<decltype(pack), itk::FixedArray<T,3>>::value, "auto -> FixedArray!");
    BOOST_REQUIRE(pack.Size() == 3);
    BOOST_CHECK(pack[0] == 1);
    BOOST_CHECK(pack[1] == 2);
    BOOST_CHECK(pack[2] == 3);
  }

  {
    RNG<T> rng;
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      in_array_type in;
      in[0] = x;
      in[1] = y;
      auto pack = otb::auto_repack(in, z);
      static_assert(std::is_same<decltype(pack), itk::FixedArray<T,3>>::value, "auto -> FixedArray!");
      BOOST_REQUIRE(pack.Size() == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }

    // With a static array
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      T in[] = {x, y};
      auto pack = otb::auto_repack(in, z);
      static_assert(std::is_same<decltype(pack), itk::FixedArray<T,3>>::value, "auto -> FixedArray!");
      BOOST_REQUIRE(pack.Size() == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }

    // With an std::array
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      std::array<T, 2> in {{x, y}};
      auto pack = otb::auto_repack(in, z);
      static_assert(std::is_same<decltype(pack), std::array<T,3>>::value, "auto -> FixedArray!");
      BOOST_REQUIRE(pack.size() == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }
  }
}

template <typename T>
void auto_test_tab2_plus_z()
{
  using in_array_type  = itk::FixedArray<T, 2>;
  using out_array_type = itk::FixedArray<T, 3>;

  {
    in_array_type in;
    in[0] = 1;
    in[1] = 2;
    auto pack = otb::auto_repack(in, 3);
    BOOST_REQUIRE(pack.Size() == 3);
    BOOST_CHECK(pack[0] == 1);
    BOOST_CHECK(pack[1] == 2);
    BOOST_CHECK(pack[2] == 3);
  }

  {
    RNG<T> rng;
    // With itk::FixedArray
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      in_array_type in;
      in[0] = x;
      in[1] = y;
      auto pack = otb::auto_repack(in, z);
      static_assert(std::is_same<decltype(pack), itk::FixedArray<T,3>>::value, "auto -> itk::FixedArray!");
      BOOST_REQUIRE(pack.Size() == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }

    // With itk::Point
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      itk::Point<T, 2> in;
      in[0] = x;
      in[1] = y;
      auto pack = otb::auto_repack(in, z);
      static_assert(std::is_same<decltype(pack), itk::Point<T,3>>::value, "auto -> itk::Point!");
      BOOST_REQUIRE(pack.Size() == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }

    // With a static array
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      T in[] = {x, y};
      auto pack = otb::repack<out_array_type>(in, z);
      static_assert(std::is_same<decltype(pack), itk::FixedArray<T,3>>::value, "auto -> FixedArray!");
      BOOST_REQUIRE(pack.Size() == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }

    // With an std::array
    for (int i=0; i<100; ++i)
    {
      auto const x = rng.generate();
      auto const y = rng.generate();
      auto const z = rng.generate();

      std::array<T, 2> in {{x, y}};
      auto pack = otb::repack<out_array_type>(in, z);
      static_assert(std::is_same<decltype(pack), std::array<T,3>>::value, "auto -> std::array!");
      BOOST_REQUIRE(pack.size() == 3);
      BOOST_REQUIRE(pack[0] == x);
      BOOST_REQUIRE(pack[1] == y);
      BOOST_REQUIRE(pack[2] == z);
    }
  }
}

BOOST_AUTO_TEST_CASE(Pack2to3Ints)
{
  test_tab2_plus_z<int>();
}

BOOST_AUTO_TEST_CASE(Pack2to3Doubles)
{
  test_tab2_plus_z<double>();
}
