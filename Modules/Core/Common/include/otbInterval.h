/*
 * Copyright(C) 2005-2020 Centre National d'Etudes Spatiales(CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https: //www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbInterval_h
#define otbInterval_h

#include "itkIntTypes.h"
#include <cassert>
#include <ostream>

namespace otb
{
/** Simplified index interval.
 * Inspired by `boost::numeric::interval`.
 *
 * \invariant Can be empty.
 * \invariant lower <= upper
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 */
class Interval
{
public:
  using IndexType = itk::IndexValueType;
  using SizeType  = itk::SizeValueType;

  /** Init constructor from the pair of its extremities. */
  constexpr Interval(IndexType l, IndexType u) noexcept
    : m_lower(l), m_upper(u)
    {
      assert(l <= u);
    }

  /** Alternate factory function from a position and a length. */
  static constexpr Interval OfLength(IndexType low, SizeType len) noexcept{
    return Interval{low, IndexType(low+len)};
  }

  constexpr SizeType size() const noexcept
  { return m_upper - m_lower; }

  constexpr bool empty() const noexcept
  { return m_lower == m_upper; }

  constexpr IndexType lower() const noexcept
  { return m_lower; }

  constexpr IndexType upper() const noexcept
  { return m_upper; }

  /** Computes the intersection between two interals.
   * @return their intersection
   * @return {0,0} if theyr don't intersect.
   *
   * @note this function is an hidden friend
   */
  friend constexpr Interval intersect(
      Interval const& lhs, Interval const& rhs) noexcept
  {
    auto const low = std::max(lhs.lower(), rhs.lower());
    auto const upp = std::min(lhs.upper(), rhs.upper());

    return low <= upp ? Interval{low, upp} : Interval{0,0};
  }

  /** Stream inserter for intervals.
   * @note this function is an hidden friend
   */
  friend std::ostream & operator<<(std::ostream & os, Interval const& v)
  {
    return os << '[' << v.lower() << ".." << v.upper() << '[';
  }

private:
  IndexType m_lower;
  IndexType m_upper;
};
} // otb namespace

#endif  // otbInterval_h
