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

#ifndef otbAlgoEqualRangeInterval_h
#define otbAlgoEqualRangeInterval_h

#include <algorithm>  // std::lower_bound, std::upper_bound
#include <cassert>
#include <functional> // std::less
#include <utility>    // std::pair

namespace otb
{
/**
 * Adaptation of `std::equal_range` for finding interval bounds.
 * \tparam RandomAccessIterator Iterator type that should model `std::random_access_iterator`.
 * \tparam T                    Type of the interval bounds
 * \tparam Comp                 Comparison function, defaults to `std::less<>`
 * \param[in] first  Start of the range where bounds are searched
 * \param[in] last   Past-end of the range where bounds are searched
 * \param[in] lo     Low bound of the interval
 * \param[in] hi     High bound of the interval
 * \param[in] cmp    Comparison predicate
 *
 * \return a pair of iterators `[it_low, it_high[` such as:
 * - `∀ it ∈ [first, it_low[  =>  cmp(*it, lo)`
 * - `∀ it ∈ [it_low, last[   => !cmp(*it, lo)`
 * - `∀ it ∈ [first, it_high[ => !cmp(hi, *it)`
 * - `∀ it ∈ [it_high, last[  =>  cmp(hi, *it)`
 *
 * Somehow, `it_low` is computed with `std::lower_bound`, and `it_hi` is computed with
 * `std::upper_bound`.
 *
 * \pre `cmp(lo, hi)`
 * \pre `[first, last[` shall be partitioned according to `cmp` predicate around `lo` and around
 *      `high`. If the range is sorted according to `cmp`, then this also implies it's partitionned.
 * \throw None (it depends on `Cmp`...)
 */
template <
    typename RandomAccessIterator,
    typename T,
    typename Comp = std::less<typename RandomAccessIterator::value_type>>
inline
std::pair<RandomAccessIterator, RandomAccessIterator>
equal_range_interval(
    RandomAccessIterator first, RandomAccessIterator last,
    T lo, T hi,
    Comp cmp = Comp{}
)
{
  // assert(std::is_sorted(first, last, cmp));
  auto len = std::distance(first, last);
  while (len > 0) {
    auto half   = len / 2;
    auto middle = first + half;
    if (cmp(*middle, lo)) {
      first = middle + 1;
      len -= half + 1;
    } else if (cmp(hi, *middle)) {
      len = half;
    } else {
      auto left  = std::lower_bound(first, middle, lo, cmp);
      auto right = std::upper_bound(middle + 1, first + len, hi, cmp);
      return {left, right};
    }
  }
  return {first, first};
}
} // namespace otb

#endif // otbAlgoEqualRangeInterval_h
