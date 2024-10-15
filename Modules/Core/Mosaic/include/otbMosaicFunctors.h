/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2016-2019 IRSTEA
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

#ifndef MODULES_REMOTE_MOSAIC_INCLUDE_OTBMOSAICFUNCTORS_H_
#define MODULES_REMOTE_MOSAIC_INCLUDE_OTBMOSAICFUNCTORS_H_

#include <array>
#include "vnl/vnl_matrix.h"
#include "vcl_compiler.h"

namespace otb
{

namespace Functor
{

/**
 * \class RGB2LAB Functor
 * \brief Base class for converting RGB into LAB color space (Ruderman et al.)
 *
 * \ingroup OTBMosaic
 */
template <class TInput, class TOutput>
class RGB2LAB
{
public:
  RGB2LAB()
  {
    M.set_size(3, 3);
    M[0][0] = 0.3811;
    M[0][1] = 0.5783;
    M[0][2] = 0.0406;
    M[1][0] = 0.1967;
    M[1][1] = 0.7244;
    M[1][2] = 0.0790;
    M[2][0] = 0.0241;
    M[2][1] = 0.1288;
    M[2][2] = 0.8531;

    D1.set_size(3, 3);
    D1.fill(0.0);
    D1[0][0] = 1.0 / vcl_sqrt(3.0);
    D1[1][1] = 1.0 / vcl_sqrt(6.0);
    D1[2][2] = 1.0 / vcl_sqrt(2.0);

    D2.set_size(3, 3);
    D2.fill(1.0);
    D2[1][2] = -2.0;
    D2[2][1] = -1.0;
    D2[2][2] = 0.0;
  }

  ~RGB2LAB()
  {
  }

  bool operator!=(const RGB2LAB&) const
  {
    return false;
  }

  bool operator==(const RGB2LAB& other) const
  {
    return !(*this != other);
  }

  inline TOutput operator()(const TInput& A) const
  {
    TOutput output;

    output.SetSize(3);
    if (A[0] == 0 && A[1] == 0 && A[2] == 0)
    {
      output.Fill(0);
      return output;
    }

    // RGB
    vnl_matrix<double> rgb(3, 1);
    rgb[0][0] = A[0];
    rgb[1][0] = A[1];
    rgb[2][0] = A[2];

    // LMS
    vnl_matrix<double> lms(3, 1);
    lms = M * rgb;

    // LMS (log10)
    const double log10 = vcl_log(10);
    lms[0][0]          = vcl_log(lms[0][0]) / log10;
    lms[1][0]          = vcl_log(lms[1][0]) / log10;
    lms[2][0]          = vcl_log(lms[2][0]) / log10;

    // LAB
    vnl_matrix<double> lab(3, 1);
    lab = D1 * (D2 * lms);

    output[0] = lab[0][0];
    output[1] = lab[1][0];
    output[2] = lab[2][0];

    return output;
  }

  size_t OutputSize(const std::array<size_t, 1>&) const
  {
    return 3;
  }

private:
  vnl_matrix<double> M;
  vnl_matrix<double> D1;
  vnl_matrix<double> D2;
};

/**
 * \class LAB2RGB Functor
 * \brief Base class for converting LAB into RGB color space (Ruderman et al.)
 *
 * TODO: invert the function RGB2LAB than using the hardcoded one
 *
 * \ingroup OTBMosaic
 */
template <class TInput, class TOutput>
class LAB2RGB
{
public:
  LAB2RGB()
  {
    M.set_size(3, 3);
    M[0][0] = 4.4687;
    M[0][1] = -3.5887;
    M[0][2] = 0.1197;
    M[1][0] = -1.2197;
    M[1][1] = 2.3831;
    M[1][2] = -0.1626;
    M[2][0] = 0.0579;
    M[2][1] = -0.2584;
    M[2][2] = 1.1934;

    D1.set_size(3, 3);
    D1.fill(0.0);
    D1[0][0] = 1.0 / vcl_sqrt(3.0);
    D1[1][1] = 1.0 / vcl_sqrt(6.0);
    D1[2][2] = 1.0 / vcl_sqrt(2.0);

    D2.set_size(3, 3);
    D2.fill(1.0);
    D2[1][2] = -1.0;
    D2[2][1] = -2.0;
    D2[2][2] = 0.0;
  }

  ~LAB2RGB()
  {
  }

  bool operator!=(const LAB2RGB&) const
  {
    return false;
  }

  bool operator==(const LAB2RGB& other) const
  {
    return !(*this != other);
  }

  inline TOutput operator()(const TInput& A) const
  {
    TOutput output;

    output.SetSize(3);

    if (A[0] == 0 && A[1] == 0 && A[2] == 0)
    {
      output.Fill(0);
      return output;
    }
    // LAB
    vnl_matrix<double> lab(3, 1);
    lab[0][0] = A[0];
    lab[1][0] = A[1];
    lab[2][0] = A[2];

    // LMS
    vnl_matrix<double> lms(3, 1);
    lms       = D2 * (D1 * lab);
    lms[0][0] = vcl_pow(10.0, lms[0][0]);
    lms[1][0] = vcl_pow(10.0, lms[1][0]);
    lms[2][0] = vcl_pow(10.0, lms[2][0]);

    // RGB
    vnl_matrix<double> rgb(3, 1);
    rgb = M * lms;

    output[0] = rgb[0][0];
    output[1] = rgb[1][0];
    output[2] = rgb[2][0];

    return output;
  }

  inline size_t OutputSize(const std::array<size_t, 1>&) const
  {
    return 3;
  }

private:
  vnl_matrix<double> M;
  vnl_matrix<double> D1;
  vnl_matrix<double> D2;
};

} // namespace functor
} // namespace otb
#endif /* MODULES_REMOTE_MOSAIC_INCLUDE_OTBMOSAICFUNCTORS_H_ */
