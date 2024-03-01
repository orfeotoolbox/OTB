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
#ifndef otbLayouts_h
#define otbLayouts_h

#include "otbExtents.h"

namespace otb
{
namespace details
{
namespace policy
{
/** Helper class for defining `layout_*::mapping` types from C++ p0009r9 draft
 * proposal.
 *
 * \tparam Extents %extents type.
 * Only provides copy, move, `is_unique()`, `is_strided()`, and hold the
 * `extents`
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 */
template <typename Extents>
struct root_layout_mapping
{
  using index_type = typename Extents::index_type;

  constexpr root_layout_mapping()                                      noexcept = default;
  constexpr root_layout_mapping(root_layout_mapping const&)            noexcept = default;
  constexpr root_layout_mapping(root_layout_mapping     &&)            noexcept = default;
  constexpr root_layout_mapping& operator=(root_layout_mapping const&) noexcept = default;
  constexpr root_layout_mapping& operator=(root_layout_mapping     &&) noexcept = default;

  constexpr root_layout_mapping(Extents const& e) noexcept
    : m_extents(e) {}

  ~root_layout_mapping() = default;

  constexpr Extents const& extents() const noexcept { return m_extents ; }

  static constexpr bool is_always_unique()     noexcept { return true ; }
  static constexpr bool is_always_strided()    noexcept { return true ; }

  constexpr bool is_unique()     const noexcept { return true ; }
  constexpr bool is_strided()    const noexcept { return true ; }

private:
  Extents m_extents;
};

/** Helper policy for defining `layout_left::mapping` types from C++ p0009r9
 * draft proposal.
 *
 * \tparam Extents %extents type.
 * Provides everything but:
 * - `required_span_size()`
 * - `operator()`
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 */
template <typename Extents>
struct left_ : root_layout_mapping<Extents>
{
  using index_type = typename root_layout_mapping<Extents>::index_type;
  using root_layout_mapping<Extents>::root_layout_mapping;

  constexpr index_type stride(const size_t r) const noexcept
  {
    assert(r < this->extents().rank());
    index_type res = 1;
    for(size_t k = 0 ; k < r; ++k)
      res *= this->extents().extent(k);
    return res;
  }
  static constexpr bool is_always_contiguous() noexcept { return true; }
  constexpr bool is_contiguous() const noexcept { return true; }
};

/** Helper policy for defining `layout_right::mapping` types from C++ p0009r9
 * draft proposal.
 *
 * \tparam Extents %extents type.
 * Provides everything but:
 * - `required_span_size()`
 * - `operator()`
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 */
template <typename Extents>
struct right_ : root_layout_mapping<Extents>
{
  using index_type = typename root_layout_mapping<Extents>::index_type;
  using root_layout_mapping<Extents>::root_layout_mapping;

  constexpr index_type stride(const size_t r) const noexcept
  {
    assert(r < this->extents().rank());
    index_type res = 1;
    for(size_t k = r+1 ; k < this->extents().rank(); ++k)
      res *= this->extents().extent(k);
    return res;
  }
  static constexpr bool is_always_contiguous() noexcept { return true; }
  constexpr bool is_contiguous() const noexcept { return true; }
};

/** Helper policy for defining `layout_stride::mapping` types from C++ p0009r9
 * draft proposal.
 *
 * \tparam Extents %extents type.
 * Provides everything but:
 * - `required_span_size()`
 * - `operator()`
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 * \warning this class is partially implemented. `is_contiguous()` isn't
 * implemented.
 */
template <typename Extents>
struct stride_ : root_layout_mapping<Extents>
{
  using index_type = typename root_layout_mapping<Extents>::index_type;
  using stride_t = std::array<index_type, Extents::rank()> ;

  // constructors
  using root_layout_mapping<Extents>::root_layout_mapping;
  constexpr stride_(Extents const& e, stride_t const& s) noexcept
    : root_layout_mapping<Extents>(e)
    , m_strides(s)
    , m_contigous(false)
    {
      // TODO: implement m_contigous computation
    }

  constexpr index_type stride(const size_t R) const noexcept
  {
    assert(R < this->extents().rank());
    return m_strides[R];
  }

  static constexpr bool is_always_contiguous() noexcept { return false; }
  constexpr bool is_contiguous() const noexcept { return m_contigous; }

private:
  stride_t m_strides;
  bool     m_contigous;
};

} // (otb::details::)policy namespace

/** Helper class for defining `layout_*` types from C++ p0009r9 draft proposal.
 *
 * \tparam LayoutPolicy Policy that defines:
 * - copying, moving
 * - `extents()`
 * - `is_unique()`, `is_always_unique()`
 * - `is_contiguous()`, `is_always_contiguous()`
 * - `is_strided()`, `is_always_strided()`
 * - `stride(r)`
 *
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 */
template <template <typename> class LayoutPolicy>
class generic_layout
{
public:
  template <typename Extents>
  class mapping : public LayoutPolicy<Extents>
  {
  public:
    using extents_type = Extents;
    using index_type   = typename LayoutPolicy<Extents>::index_type;
    using LayoutPolicy<Extents>::LayoutPolicy;

    template<class... Indices>
    constexpr index_type operator()(Indices... i) const noexcept
    {
      std::array<index_type, extents_type::rank()> idx{i...};
      index_type r = 0;
      for (size_t k = 0 ; k < extents_type::rank() ; ++k)
      {
        r += idx[k] * this->stride(k);
      }
      return r;
    }

    constexpr index_type required_span_size() const noexcept
    {
      index_type r = 0;
      for (size_t k = 0 ; k < Extents::rank() ; ++k)
        r = std::max(r, this->stride(k) * this->extents().extent(k));
      return r;
      // TODO: use `r *= this->extents().extent(k);` for contiguous cases
    }
  };
};

} // (otb::)details namespace

/** Left layout implementing C++ p0009r9 draft proposal.
 * Gives a layout mapping where the left-most extent is stride one and strides
 * increase left-to-right as the product of extents.
 *
 * Best used to iterate _à la Fortran_:
 * \code
 * for (i :....)
 *    for (j :....)
 *        mapping(j, i)
 * \endcode
 *
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 */
using layout_left   = details::generic_layout<details::policy::left_>;

/** Right layout implementing C++ p0009r9 draft proposal.
 * Gives a layout mapping where the right-most extent is stride one and strides
 * increase right-to-left as the product of extents.
 *
 * Best used to iterate _à la C_:
 * \code
 * for (i :....)
 *    for (j :....)
 *        mapping(i, j)
 * \endcode
 *
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 */
using layout_right  = details::generic_layout<details::policy::right_>;

/** Strided layout implementing C++ p0009r9 draft proposal.
 * Gives a layout mapping where the strides are user defined.
 *
 * Best used for complex data organisation.
 *
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 */
using layout_stride = details::generic_layout<details::policy::stride_>;

} // otb namespace

#endif // otbLayouts_h
