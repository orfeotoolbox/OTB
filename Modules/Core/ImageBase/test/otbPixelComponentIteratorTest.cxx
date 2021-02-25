/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#define BOOST_TEST_MODULE "otb::PixelComponentIterator unit testing"
#define BOOST_TEST_DYN_LINK
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include <boost/test/unit_test.hpp>
#pragma GCC diagnostic pop
#else
#include <boost/test/unit_test.hpp>
#endif
#include "otbPixelComponentIterator.h"
#include <array>
#include <numeric>

namespace tt = boost::test_tools;

using namespace otb;

/*===========================================================================*/
/*==================================[ UTs ]==================================*/
/*===========================================================================*/

/*===========================================================================*/
// ===========< Create & Read tests
BOOST_AUTO_TEST_CASE(PixelComponentIterator_Scalar_Creation)
{
  using PixelType = double;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(std::is_arithmetic<PixelType>::value, "Should compile");

  PixelType p{0};

  auto pr = PixelRange(p);
  BOOST_REQUIRE_EQUAL(pr.shallow_size(), 1U);
  BOOST_REQUIRE_EQUAL(pr.size(), 1U);
  for (auto c : pr)
  {
    BOOST_CHECK_EQUAL(c, 0);
  }
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Complex_Creation)
{
  using PixelType = std::complex<double>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(boost::is_complex<PixelType>::value, "Should compile");

  PixelType const p{12, 42};

  std::array<double, 2> ref = {12, 42};

  auto pr = PixelRange(p);
  BOOST_REQUIRE_EQUAL(pr.shallow_size(), 2U);
  BOOST_REQUIRE_EQUAL(pr.size(), 2U);
  BOOST_REQUIRE_EQUAL(mpl::GetNumberOfComponents(p), 2U);
  BOOST_TEST(pr == ref, tt::per_element());
}

#if 1
BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV_Creation)
{
  using PixelType = itk::VariableLengthVector<double>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(itk::mpl::IsArray<PixelType>::Value, "Should compile");

  PixelType p{4};
  p[0] = 0;
  p[1] = 1;
  p[2] = 2;
  p[3] = 3;

  std::array<double, 4> ref = {0, 1, 2, 3};

  auto pr = PixelRange(p);
  BOOST_REQUIRE_EQUAL(pr.shallow_size(), 4U);
  BOOST_REQUIRE_EQUAL(pr.size(), 4U);
  BOOST_TEST(pr == ref, tt::per_element());
}
#endif

#if 0
BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV_Expr_Creation)
{
  using PixelType = itk::VariableLengthVector<double>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(itk::mpl::IsArray<PixelType>::Value, "Should compile");

  PixelType p{4};
  p[0] = 0;
  p[1] = 1;
  p[2] = 2;
  p[3] = 3;

  std::array<double, 4> ref = {0, 2, 4, 6};

  static_assert(!std::is_reference<decltype((p+p)[0])>::value);

  auto p2 = p + p;
  auto pr = PixelRange(p2);
  // static_assert(std::is_const<decltype(pr)::PixelType>::value, "Should compile");
  // static_assert(!std::is_lvalue_reference<decltype(*pr.begin())>::value, "Results of dereferenced VLVE iterators shall NOT be lvalue-ref");

  BOOST_REQUIRE_EQUAL(pr.shallow_size(), 4U);
  BOOST_TEST(pr == ref, tt::per_element());
}
#endif

/*===========================================================================*/
// ===========< Write tests
BOOST_AUTO_TEST_CASE(PixelComponentIterator_Scalar_Set)
{
  using PixelType = double;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(std::is_arithmetic<PixelType>::value, "Should compile");

  PixelType p{0};

  auto pr = PixelRange(p);
  BOOST_REQUIRE_EQUAL(pr.shallow_size(), 1U);
  BOOST_REQUIRE_EQUAL(pr.size(), 1U);
  std::fill(std::begin(pr), std::end(pr), 42);
  for (auto c : pr)
  {
    BOOST_CHECK_EQUAL(c, 42);
  }
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Complex_Set)
{
  using PixelType = std::complex<double>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(boost::is_complex<PixelType>::value, "Should compile");

  PixelType p{0};

  std::array<double, 2> ref = {42, 43};

  auto pr = PixelRange(p);
  BOOST_REQUIRE_EQUAL(pr.shallow_size(), 2U);
  BOOST_REQUIRE_EQUAL(pr.size(), 2U);
  std::iota(std::begin(pr), std::end(pr), 42);
  BOOST_TEST(pr == ref, tt::per_element());
}

#if 1
BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV_Set)
{
  using PixelType = itk::VariableLengthVector<double>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(itk::mpl::IsArray<PixelType>::Value, "Should compile");

  PixelType p{4};
  p[0] = 0;
  p[1] = 1;
  p[2] = 2;
  p[3] = 3;

  std::array<double, 4> ref = {42, 43, 44, 45};

  auto pr = PixelRange(p);
  BOOST_REQUIRE_EQUAL(pr.shallow_size(), 4U);
  BOOST_REQUIRE_EQUAL(pr.size(), 4U);
  std::iota(std::begin(pr), std::end(pr), 42);
  BOOST_TEST(pr == ref, tt::per_element());
}
#endif

/*===========================================================================*/
// ===========< Convert tests
template <typename InPixelRange, typename OutPixelRange>
void copy(InPixelRange const in, OutPixelRange out)
{
  auto       first  = in.begin();
  auto const end    = in.end();
  auto       dfirst = out.begin();
  auto const dend   = out.end();

  using OutputPixelValueType = std::decay_t<decltype(*dfirst)>;
  for (; first!=end && dfirst!=dend ; ++first, ++dfirst)
  {
    *dfirst = static_cast<OutputPixelValueType>(*first);
  }
  for (; dfirst != dend ; ++dfirst)
  { // complete with extra 0 (case where we have less input bands, and we store into complex)
    *dfirst = OutputPixelValueType{};
  }
}

// ===========< Scalar -> Scalar
template <typename InPixelType, typename OutPixelType>
void PixelComponentIterator_Scalar_Convert()
{
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(std::is_arithmetic<InPixelType>::value, "Should compile");
  static_assert(std::is_arithmetic<OutPixelType>::value, "Should compile");

  InPixelType const in{42};
  OutPixelType out{0};

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 42);
  }
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Scalar_Convert_same_type)
{
  PixelComponentIterator_Scalar_Convert<int, int>();
  PixelComponentIterator_Scalar_Convert<float, float>();
  PixelComponentIterator_Scalar_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Scalar_Convert_widen_type)
{
  PixelComponentIterator_Scalar_Convert<int, long long>();
  PixelComponentIterator_Scalar_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Scalar_Convert_narrow_type)
{
  PixelComponentIterator_Scalar_Convert<long long, int>();
  PixelComponentIterator_Scalar_Convert<double, float>();
}

// ===========< VLV -> VLV
template <typename InComponentType, typename OutComponentType>
void PixelComponentIterator_VLV_Convert()
{
  using InPixelType  = itk::VariableLengthVector<InComponentType>;
  using OutPixelType = itk::VariableLengthVector<OutComponentType>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(itk::mpl::IsArray<InPixelType>::Value, "Should compile");
  static_assert(itk::mpl::IsArray<OutPixelType>::Value, "Should compile");

  InPixelType in(4);
  in[0] = 42;
  in[1] = 43;
  in[2] = 44;
  in[3] = 45;
  OutPixelType out(4);
  out[0] = 0;
  out[1] = 0;
  out[2] = 0;
  out[3] = 0;

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  std::array<double, 4> ref = {42, 43, 44, 45};

  BOOST_TEST(PixelRange(out) == ref, tt::per_element());
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV_Convert_same_type)
{
  PixelComponentIterator_VLV_Convert<int, int>();
  PixelComponentIterator_VLV_Convert<float, float>();
  PixelComponentIterator_VLV_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV_Convert_widen_type)
{
  PixelComponentIterator_VLV_Convert<int, long long>();
  PixelComponentIterator_VLV_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV_Convert_narrow_type)
{
  PixelComponentIterator_VLV_Convert<long long, int>();
  PixelComponentIterator_VLV_Convert<double, float>();
}

// ===========< Complex -> Complex
template <typename InComponentType, typename OutComponentType>
void PixelComponentIterator_Complex_Convert()
{
  using InPixelType  = std::complex<InComponentType>;
  using OutPixelType = std::complex<OutComponentType>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(boost::is_complex<InPixelType>::value, "Should compile");
  static_assert(boost::is_complex<OutPixelType>::value, "Should compile");

  InPixelType const in{42, 43};
  OutPixelType out{};

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  std::array<double, 2> ref = {42, 43};

  BOOST_TEST(PixelRange(out) == ref, tt::per_element());
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Complex_Convert_same_type)
{
  PixelComponentIterator_Complex_Convert<float, float>();
  PixelComponentIterator_Complex_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Complex_Convert_widen_type)
{
  PixelComponentIterator_Complex_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Complex_Convert_narrow_type)
{
  PixelComponentIterator_Complex_Convert<double, float>();
}

// ===========< Scalar -> VLV
template <typename InPixelType, typename OutComponentType>
void PixelComponentIterator_Scalar2VLV_Convert()
{
  using OutPixelType = itk::VariableLengthVector<OutComponentType>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(std::is_arithmetic<InPixelType>::value, "Should compile");
  static_assert(itk::mpl::IsArray<OutPixelType>::Value, "Should compile");

  InPixelType const in = 42;
  OutPixelType out(1);
  out[0] = 0;

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  std::array<double, 1> ref = {42};

  BOOST_TEST(PixelRange(out) == ref, tt::per_element());
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Scalar2VLV_Convert_same_type)
{
  PixelComponentIterator_Scalar2VLV_Convert<int, int>();
  PixelComponentIterator_Scalar2VLV_Convert<float, float>();
  PixelComponentIterator_Scalar2VLV_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Scalar2VLV_Convert_widen_type)
{
  PixelComponentIterator_Scalar2VLV_Convert<int, long long>();
  PixelComponentIterator_Scalar2VLV_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Scalar2VLV_Convert_narrow_type)
{
  PixelComponentIterator_Scalar2VLV_Convert<long long, int>();
  PixelComponentIterator_Scalar2VLV_Convert<double, float>();
}

// ===========< VLV -> Scalar
template <typename InComponentType, typename OutPixelType>
void PixelComponentIterator_VLV2Scalar_Convert()
{
  using InPixelType = itk::VariableLengthVector<InComponentType>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(itk::mpl::IsArray<InPixelType>::Value, "Should compile");
  static_assert(std::is_arithmetic<OutPixelType>::value, "Should compile");

  InPixelType in(1);
  in[0] = 42;
  OutPixelType out = 0;

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  std::array<double, 1> ref = {42};

  BOOST_TEST(PixelRange(out) == ref, tt::per_element());
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2Scalar_Convert_same_type)
{
  PixelComponentIterator_VLV2Scalar_Convert<int, int>();
  PixelComponentIterator_VLV2Scalar_Convert<float, float>();
  PixelComponentIterator_VLV2Scalar_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2Scalar_Convert_widen_type)
{
  PixelComponentIterator_VLV2Scalar_Convert<int, long long>();
  PixelComponentIterator_VLV2Scalar_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2Scalar_Convert_narrow_type)
{
  PixelComponentIterator_VLV2Scalar_Convert<long long, int>();
  PixelComponentIterator_VLV2Scalar_Convert<double, float>();
}

// ===========< Complex -> VLV
template <typename InComponentType, typename OutComponentType>
void PixelComponentIterator_Complex2VLV_Convert()
{
  using InPixelType  = std::complex<InComponentType>;
  using OutPixelType = itk::VariableLengthVector<OutComponentType>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(boost::is_complex<InPixelType>::value, "Should compile");
  static_assert(itk::mpl::IsArray<OutPixelType>::Value, "Should compile");

  InPixelType const in{42, 43};
  OutPixelType out(2);
  out[0] = 0;
  out[1] = 0;

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  std::array<double, 2> ref = {42, 43};

  BOOST_TEST(PixelRange(in)  == ref, tt::per_element());
  BOOST_TEST(PixelRange(out) == ref, tt::per_element());
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Complex2VLV_Convert_same_type)
{
  PixelComponentIterator_Complex2VLV_Convert<float, float>();
  PixelComponentIterator_Complex2VLV_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Complex2VLV_Convert_widen_type)
{
  PixelComponentIterator_Complex2VLV_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_Complex2VLV_Convert_narrow_type)
{
  PixelComponentIterator_Complex2VLV_Convert<double, float>();
}

// ===========< VLV -> Complex
template <typename InComponentType, typename OutComponentType>
void PixelComponentIterator_VLV2Complex_Convert()
{
  using InPixelType = itk::VariableLengthVector<InComponentType>;
  using OutPixelType = std::complex<InComponentType>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(itk::mpl::IsArray<InPixelType>::Value, "Should compile");
  static_assert(boost::is_complex<OutPixelType>::value, "Should compile");

  InPixelType in(2);
  in[0] = 42;
  in[1] = 43;
  OutPixelType out {};

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  std::array<double, 2> ref = {42, 43};

  BOOST_REQUIRE_EQUAL(PixelRange(in).shallow_size(), 2U);
  BOOST_REQUIRE_EQUAL(PixelRange(in).size(), 2U);
  BOOST_TEST(PixelRange(in) == ref, tt::per_element());
  BOOST_REQUIRE_EQUAL(PixelRange(out).shallow_size(), 2U);
  BOOST_REQUIRE_EQUAL(PixelRange(out).size(), 2U);
  BOOST_TEST(PixelRange(out) == ref, tt::per_element());
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2Complex_Convert_same_type)
{
  PixelComponentIterator_VLV2Complex_Convert<float, float>();
  PixelComponentIterator_VLV2Complex_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2Complex_Convert_widen_type)
{
  PixelComponentIterator_VLV2Complex_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2Complex_Convert_narrow_type)
{
  PixelComponentIterator_VLV2Complex_Convert<double, float>();
}

// ===========< VLV<Complex> -> VLV
template <typename InComponentType, typename OutComponentType>
void PixelComponentIterator_VLVComplex2VLV_Convert()
{
  using InPixelType  = itk::VariableLengthVector<std::complex<InComponentType>>;
  using OutPixelType = itk::VariableLengthVector<OutComponentType>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(itk::mpl::IsArray<InPixelType>::Value, "Should compile");
  static_assert(itk::mpl::IsArray<OutPixelType>::Value, "Should compile");

  InPixelType  in(3);
  in[0] = {42, 43};
  in[1] = {44, 45};
  in[2] = {46, 47};
  OutPixelType out(6);
  out[0] = 0;
  out[1] = 0;
  out[2] = 0;
  out[3] = 0;
  out[4] = 0;
  out[5] = 0;

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  std::array<InComponentType, 6> iref = {42, 43, 44, 45, 46, 47};
  std::array<OutComponentType, 6> ref = {42, 43, 44, 45, 46, 47};

  BOOST_REQUIRE_EQUAL(PixelRange(in).shallow_size(), 3U);
  BOOST_REQUIRE_EQUAL(PixelRange(in).size(), 6U);
  BOOST_TEST(PixelRange(in) == iref, tt::per_element());

  BOOST_REQUIRE_EQUAL(PixelRange(out).shallow_size(), 6U);
  BOOST_REQUIRE_EQUAL(PixelRange(out).size(), 6U);
  BOOST_TEST(PixelRange(out) == ref, tt::per_element());
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLVComplex2VLV_Convert_same_type)
{
  PixelComponentIterator_VLVComplex2VLV_Convert<float, float>();
  PixelComponentIterator_VLVComplex2VLV_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLVComplex2VLV_Convert_widen_type)
{
  PixelComponentIterator_VLVComplex2VLV_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLVComplex2VLV_Convert_narrow_type)
{
  PixelComponentIterator_VLVComplex2VLV_Convert<double, float>();
}

// ===========< VLV -> VLV<Complex>
template <typename InComponentType, typename OutComponentType>
void PixelComponentIterator_VLV2VLVComplex_Convert()
{
  using InPixelType  = itk::VariableLengthVector<InComponentType>;
  using OutPixelType = itk::VariableLengthVector<std::complex<OutComponentType>>;
  // using PixelComponentIterator_t = PixelComponentIterator<PixelType>;
  static_assert(itk::mpl::IsArray<InPixelType>::Value, "Should compile");
  static_assert(itk::mpl::IsArray<OutPixelType>::Value, "Should compile");

  InPixelType  in(6);
  in[0] = 42;
  in[1] = 43;
  in[2] = 44;
  in[3] = 45;
  in[4] = 46;
  in[5] = 47;
  OutPixelType out(3);
  out[0] = 0;
  out[1] = 0;
  out[2] = 0;

  for (auto c : PixelRange(out))
  {
    BOOST_CHECK_EQUAL(c, 0);
  }

  copy(PixelRange(in), PixelRange(out));

  std::array<InComponentType, 6> iref = {42, 43, 44, 45, 46, 47};
  std::array<OutComponentType, 6> ref = {42, 43, 44, 45, 46, 47};

  BOOST_REQUIRE_EQUAL(PixelRange(in).shallow_size(), 6U);
  BOOST_REQUIRE_EQUAL(PixelRange(in).size(), 6U);
  BOOST_TEST(PixelRange(in) == iref, tt::per_element());

  BOOST_REQUIRE_EQUAL(PixelRange(out).shallow_size(), 3U);
  BOOST_REQUIRE_EQUAL(PixelRange(out).size(), 6U);
  BOOST_TEST(PixelRange(out) == ref, tt::per_element());
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2VLVComplex_Convert_same_type)
{
  PixelComponentIterator_VLV2VLVComplex_Convert<float, float>();
  PixelComponentIterator_VLV2VLVComplex_Convert<double, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2VLVComplex_Convert_widen_type)
{
  PixelComponentIterator_VLV2VLVComplex_Convert<float, double>();
}

BOOST_AUTO_TEST_CASE(PixelComponentIterator_VLV2VLVComplex_Convert_narrow_type)
{
  PixelComponentIterator_VLV2VLVComplex_Convert<double, float>();
}

// TODO: Quid if VLV<double> is meant to be translated into
// VLV<complex>: same number of band in both VectorImages?
