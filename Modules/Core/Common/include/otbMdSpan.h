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
#ifndef otbMdSpan_h
#define otbMdSpan_h

#include "otbExtents.h"
#include "otbLayouts.h"
#include "otbSpan.h"
#include <type_traits>

namespace otb
{
struct narrow {};

/** Multidensional Span class inspired by C++ p0009r9 draft proposal.
 * [mdspan.basic], class template mdspan
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 * \note _AccessorPolicy_ is not implemented.
 */
template<class ElementType,
  class Extents,
  class LayoutPolicy = layout_right>
class basic_mdspan
{
public:

  // Domain and codomain types
  using extents_type     = Extents;
  using layout_type      = LayoutPolicy;
  // using accessor_type = AccessorPolicy;
  using mapping_type     = typename layout_type::template mapping<extents_type>;
  // using element_type  = typename accessor_type::element_type;
  using element_type     = ElementType;
  using value_type       = std::remove_cv_t<element_type>;
  using index_type       = ptrdiff_t ;
  using difference_type  = ptrdiff_t;
  using pointer          = element_type*;
  using reference        = element_type&;

  // [mdspan.basic.cons], basic_mdspan constructors, assignment, and destructor
  constexpr basic_mdspan()                    noexcept = default;
  constexpr basic_mdspan(basic_mdspan const&) noexcept = default;
  constexpr basic_mdspan(basic_mdspan     &&) noexcept = default;

  /** Constructs from extent list.
   * This flavour will emit _warning: narrowing conversion of_ messages when
   * passing `size_t` values instead of `ptrdiff_t` ones.
   */
  template<class... IndexType>
  explicit constexpr basic_mdspan(pointer p, IndexType... dynamic_extents) noexcept
  : m_ptr(p)
  , m_map(extents_type{dynamic_extents...})
  {}

  /** Constructs from extent list, ignore narrowing warnings.
   * This flavour will ignore _warning: narrowing conversion of_ messages
   * without relying on the global `-Wnarrowing` flag.
   *
   * Useful to directly use `size_t` values instead of `ptrdiff_t` ones.
   */
  template<class... IndexType>
  constexpr basic_mdspan(pointer p, narrow, IndexType... dynamic_extents) noexcept
  : m_ptr(p)
  , m_map(extents_type{static_cast<index_type>(dynamic_extents)...})
  {}

  /** Constructs from array of extents. */
  template<class IndexType, size_t N>
  constexpr basic_mdspan(pointer p, std::array<IndexType, N> const& dynamic_extents) noexcept
  : m_ptr(p)
  , m_map(extents_type{dynamic_extents})
  {}

  /** Constructs from a layout representation. */
  constexpr basic_mdspan(pointer p, mapping_type const& m) noexcept
    : m_ptr(p), m_map(m) {}

  template<class OtherElementType, class OtherExtents, class OtherLayoutPolicy>
  constexpr basic_mdspan(const basic_mdspan<OtherElementType, OtherExtents, OtherLayoutPolicy>& other) noexcept
  : m_ptr(other.m_ptr), m_map(other.m_map)
  {}

  ~basic_mdspan() = default;

  constexpr basic_mdspan& operator=(basic_mdspan const&) noexcept = default;
  constexpr basic_mdspan& operator=(basic_mdspan     &&) noexcept = default;

  template<class OtherElementType, class OtherExtents, class OtherLayoutPolicy>
  constexpr basic_mdspan& operator=(const basic_mdspan<OtherElementType, OtherExtents, OtherLayoutPolicy>& other) noexcept
  {
    m_ptr = other.m_ptr;
    m_map = other.m_map;
    return *this;
  }

  // [mdspan.basic.mapping], basic_mdspan mapping domain multidimensional index to access codomain element
  constexpr reference operator[](index_type) const noexcept;
  template<class... IndexType>
  constexpr reference operator()(IndexType... indices) const noexcept
  {
    return m_ptr[m_map(indices...)];
  }

  template<class IndexType, size_t N>
  constexpr reference operator()(std::array<IndexType, N> const& indices) const noexcept
  {
    return m_ptr[m_map(indices)];
  }

  // [mdspan.basic.domobs], basic_mdspan observers of the domain multidimensional index space
  static constexpr int rank() noexcept
  { return extents_type::rank(); }
  static constexpr int rank_dynamic() noexcept
  { return extents_type::rank_dynamic(); }
  static constexpr index_type static_extent(size_t k) noexcept
  { return extents_type::static_extent(k); }

  constexpr extents_type extents() const noexcept
  { return m_map.extents(); }
  constexpr index_type extent(size_t k) const noexcept
  { return extents().extent(k); }
  constexpr index_type size() const noexcept
  {
    if (rank() == 0)
      return 0;
    index_type r = 1;
    for (std::size_t k=0; k<rank(); ++k)
      r *= extent(k);
    return r;
  }
  constexpr index_type unique_size() const noexcept;

  // [mdspan.basic.codomain], basic_mdspan observers of the codomain
  constexpr Span<element_type> span() const noexcept
  { return Span<element_type>{data(), mapping().required_span_size()}; }
  constexpr pointer data() const noexcept
  { return m_ptr; }

  // [mdspan.basic.obs], basic_mdspan observers of the mapping
  static constexpr bool is_always_unique()     noexcept
  { return mapping_type::is_always_unique(); }
  static constexpr bool is_always_contiguous() noexcept
  { return mapping_type::is_always_contiguous(); }
  static constexpr bool is_always_strided()    noexcept
  { return mapping_type::is_always_strided(); }

  constexpr mapping_type mapping() const noexcept
  { return m_map; }
  constexpr bool is_unique()     const noexcept
  { return mapping().is_unique(); }
  constexpr bool is_contiguous() const noexcept
  { return mapping().is_contiguous(); }
  constexpr bool is_strided()    const noexcept
  { return mapping().is_strided(); }
  constexpr index_type stride(size_t k) const noexcept
  { return mapping().stride(k); }

private:
  pointer      m_ptr = nullptr;
  mapping_type m_map;
};

template<class T, ptrdiff_t... Extents>
using mdspan = basic_mdspan<T, extents<Extents...>>;

} // otb namespace

#endif // otbMdSpan_h
