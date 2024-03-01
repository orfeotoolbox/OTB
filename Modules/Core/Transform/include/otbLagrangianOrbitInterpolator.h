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

#ifndef otbLagrangianOrbitInterpolator_h
#define otbLagrangianOrbitInterpolator_h

#include <vector>
#include <cassert>
#include <limits>
#include <array>
#include "otbMdSpan.h"

#include "otbSARMetadata.h"
#include "otbMacro.h"

namespace otb
{

/**
 * Lagrangian Interpolator used for SAR sensor position interpolation.
 *
 * \todo Make the interpolator generic to support any kind of timed series.
 *
 * \internal
 * This implementation pre-computes time difference products between timed
 * samples, and caches their inverse.
 */
class LagrangianOrbitInterpolator
{
  using inv_den_view_t = mdspan<double, dynamic_extent, dynamic_extent>;

public:
  using PointType = Orbit::PointType;
  using TimePoint = MetaData::TimePoint;

  explicit LagrangianOrbitInterpolator(
      std::vector<Orbit> const& orbits,
      unsigned int polynomial_degree = 8)
     : m_polynomial_degree(polynomial_degree - 2)
     , m_orbits(orbits)
     {
       // Legacy implementation used to misinterpret "degree".
       // For a polynom of degree 8, 8 values shall be used:
       // \Sum_{i=0}^{n} \Mult_{j=0, j!=i=^{n} \frac{X - x_i}{x_j - x-i}
       // => 9 values for i ([0..n]), 8 values for j ([0..i[ U ]i..n])
       //
       // Let's assume a center at idx=7, and degree 8
       // legacyBegin = idx-deg/2+1 = 4
       // legacyEnd   = legacyBegin + deg - 1 = 4+8-1 = 11
       // and we loop in [legacyBegin..legacyEnd[ => 7 values
       // IOW, real degree is 6 (7-1)!
       assert(m_polynomial_degree < 30); // Lagrangian interpolator fails
                                         // miserably at 20 elements... Let's
                                         // expected less than 30 as reasonable

       m_buffer.resize(2*m_polynomial_degree * orbits.size());
       m_inv_den = inv_den_view_t(
           m_buffer.data(), narrow{},
           orbits.size(),
           2 * m_polynomial_degree);

       auto nLast = m_orbits.size();
       for (std::size_t nBegin = 0; nBegin < nLast ; ++nBegin)
       {
         auto const nEnd = std::min(nLast, nBegin + m_polynomial_degree+1);
         // ∀ i ∈ [nBegin..nEnd[
         // --> inv_den(nBegin, i) = TT_{j!=i} t[i] - t[j]
         auto const offset_max = std::min(nLast-nBegin, std::size_t(1 + m_polynomial_degree));
         for (std::size_t offset_i=0; offset_i < offset_max ; ++offset_i)
         {
           std::ptrdiff_t i = offset_i + nBegin;
           assert(i < std::ptrdiff_t(nLast));
           auto const t_i = m_orbits[i].time;

           double w = 1.;
           unsigned int j = nBegin;
           for( ; j < i ; ++j)
           {
             assert(i != j);
             auto const den = t_i - m_orbits[j].time;
             assert(den.NumberOfTicks() != 0);
             w *= den.NumberOfTicks();;
           }
           for( ++j ; j < nEnd; ++j)
           {
             assert(i != j);
             auto const den = t_i - m_orbits[j].time;
             assert(den.NumberOfTicks() != 0);
             w *= den.NumberOfTicks();;
           }
           assert(nBegin < m_orbits.size());
           assert(offset_i < 1 + m_polynomial_degree);
           assert(w != 0);
           m_inv_den(unsigned(nBegin), unsigned(offset_i)) = 1.0 / w;
         }
       }
     }

  std::pair<Orbit::PointType, Orbit::PointType> interpolatePosVel(
      TimePoint time, std::size_t nBegin, std::size_t nEnd) const
  {
    // Compute lagrangian interpolation using records from nBegin to nEnd
    assert(nEnd - nBegin < m_polynomial_degree+2);
    assert(nBegin < nEnd);

    // Time differences between time and sample[i].time
    std::array<double, 30> td1s;
    for(unsigned int i = nBegin; i < nEnd; ++i)
    {
      td1s[i-nBegin] = (time - m_orbits[i].time).NumberOfTicks();
    }

    PointType pos(0.); // Fill all with 0.
    PointType vel(0.); // Fill all with 0.

    for(unsigned int i = nBegin; i < nEnd; ++i)
    {
      double w = 1.;
      std::size_t j = nBegin;
      for( ; j != i ; ++j)
      {
        w *= td1s[j-nBegin];
      }
      ++j;
      for( ; j < nEnd; ++j)
      {
        w *= td1s[j-nBegin];
      }
      auto const den = m_inv_den(std::ptrdiff_t(nBegin), std::ptrdiff_t(i-nBegin));
      w *= den;

      pos[0]+=w*m_orbits[i].position[0];
      pos[1]+=w*m_orbits[i].position[1];
      pos[2]+=w*m_orbits[i].position[2];
      vel[0]+=w*m_orbits[i].velocity[0];
      vel[1]+=w*m_orbits[i].velocity[1];
      vel[2]+=w*m_orbits[i].velocity[2];
    }

    return {pos, vel};
  }

private:
  LagrangianOrbitInterpolator(LagrangianOrbitInterpolator const&) = delete;
  LagrangianOrbitInterpolator& operator=(LagrangianOrbitInterpolator const&) = delete;

  unsigned int m_polynomial_degree;

  std::vector<Orbit> const& m_orbits;
  std::vector<double>       m_buffer;
  inv_den_view_t            m_inv_den;
};

} // otb namespace

#endif  // otbLagrangianOrbitInterpolator_h
