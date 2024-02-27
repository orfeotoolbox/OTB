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
#ifndef otbExtents_h
#define otbExtents_h

// #include <boost/hana.hpp> // boost.hana requires boost 1.61.0
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/count.hpp>
#include <cstddef>
#include <array>
#include <algorithm>
#include <cassert>

namespace otb
{
enum : std::ptrdiff_t { dynamic_extent = -1 };

namespace details
{

template< std::ptrdiff_t ... StaticExtents >
constexpr std::size_t rank_dynamic() noexcept
{
#if 0
  return boost::hana::count(
      boost::hana::make_tuple(StaticExtents...),
      dynamic_extent) ;
#else
  using v = boost::mpl::vector_c<std::ptrdiff_t, StaticExtents...>;
  using c = boost::mpl::count<v, boost::mpl::integral_c<std::ptrdiff_t, dynamic_extent>>;
  return c::value;
#endif
}

} // details namespace


/** `mdspan.extents` type as proposed by C++ p0009r9 proposal draft.
 *
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0009r9.html
 * \sa https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation
 */
template<std::ptrdiff_t... StaticExtents>
class extents
{
public:
  // types
  using index_type = std::ptrdiff_t;

private:
    using dynamic_extents_t = std::array<index_type, details::rank_dynamic<StaticExtents...>()>;
    dynamic_extents_t dynamic_extents_;

    static constexpr size_t dynamic_rank(size_t i) noexcept
    {
        assert(i < rank());
        constexpr auto static_extent_ = std::array<index_type, rank()>{StaticExtents...};
        return std::count(&static_extent_[0], &static_extent_[i+1], dynamic_extent);
    }

public:

  // [mdspan.extents.cons], Constructors and assignment
  constexpr extents()               noexcept {}
  constexpr extents(extents const&) noexcept = default;
  constexpr extents(extents     &&) noexcept = default ;

  template<class... IndexType>
  constexpr explicit extents(index_type dn, IndexType... dynamic) noexcept
  : dynamic_extents_{dn, dynamic...}
  { static_assert( 1+sizeof...(dynamic) == rank_dynamic() , "" ); }

  template<class IndexType>
  constexpr extents(const std::array<IndexType, details::rank_dynamic<StaticExtents...>()>& dynamic) noexcept
  : dynamic_extents_(dynamic)
  {}

#if 0
  template<std::ptrdiff_t... OtherExtents>
  constexpr extents(const extents<OtherExtents...>& other) noexcept;
#endif

  constexpr extents& operator=(extents const&) noexcept = default;
  constexpr extents& operator=(extents     &&) noexcept = default;
#if 0
  template<std::ptrdiff_t... OtherExtents>
  constexpr extents& operator=(const extents<OtherExtents...>& other) noexcept;
#endif

  ~extents() = default;

  // [mdspan.extents.obs], Observers of the domain multidimensional index space
  static constexpr size_t rank() noexcept
  { return sizeof...(StaticExtents); }

  static constexpr std::size_t rank_dynamic() noexcept
  { return details::rank_dynamic<StaticExtents...>() ; }

  static constexpr index_type static_extent(size_t k) noexcept
  {
    constexpr auto se = std::array<index_type, rank()>{StaticExtents...};
    return k<rank() ? se[k] : 1;
  }

  constexpr index_type extent(size_t k) const noexcept
  {
    // less optimizer friendly than the recursive approach
    // GCC 7.1 is less efficient to resolve constexpr search, than
    // clang or GCC 6.x
    constexpr auto se = std::array<index_type, rank()>{StaticExtents...};
    return k >= rank()             ? 1
      :    se[k] == dynamic_extent ? dynamic_extents_[dynamic_rank(k)-1]
      :                              se[k]
      ;
  }

  // Tells whether an extent is fully contained in another one
  template<std::ptrdiff_t... StaticExtentsR>
  constexpr bool contains(extents<StaticExtentsR...> const& rhs) const noexcept
  {
    auto const& lhs = *this;
    assert(lhs.rank() == rhs.rank());
    const auto r = lhs.rank();
    for (std::size_t k=0; k!=r; ++k)
      if (lhs.extent(k) > rhs.extent(k))
        return false;
    return true;
  }

};

template<std::ptrdiff_t... StaticExtentsL, std::ptrdiff_t... StaticExtentsR>
constexpr bool operator==(
    extents<StaticExtentsL...> const& lhs,
    extents<StaticExtentsR...> const& rhs)
{
  if (lhs.rank() != rhs.rank())
    return false;
  const auto r = lhs.rank();
  for (std::size_t k=0; k!=r; ++k)
    if (lhs.extent(k) != rhs.extent(k))
      return false;
  return true;
}

template<std::ptrdiff_t... StaticExtentsL, std::ptrdiff_t... StaticExtentsR>
constexpr bool operator!=(
    extents<StaticExtentsL...> const& lhs,
    extents<StaticExtentsR...> const& rhs)
{
  return ! (lhs == rhs);
}

} // otb namespace

#endif // otbExtents_h
