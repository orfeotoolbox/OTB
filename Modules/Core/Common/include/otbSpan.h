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


#ifndef Span_h
#define Span_h

#include <type_traits>
#include <iterator>
#include <limits>
#include <cassert>

namespace otb
{

/** Span class inspired by C++20 standard.
 *
 * \invariant `size() == 0 or data() != nullptr`
 *
 * \note Unlike C++20 `std::span` this implementation doesn't follow Lakos
 * Rule but instead non-throwing functions are `noexcept` as suggested in
 * https://wg21.link/p1656. Beware to not expect `operator[]` to always be
 * `noexcept` as it won't be anymore once this class is deprecated in favour
 * of `std::span` in a few years.
 *
 * \note This implementation only support spans with dynamic extents. Static
 * extents are not supported (yet?)
 *
 * \todo fix RW / RO interface
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 */
template <typename T> struct Span
{
  /**\name Typedefs */
  //@{
  using element_type           = T;
  using value_type             = std::remove_cv_t<T>;
  using index_type             = std::size_t;
  using difference_type        = std::ptrdiff_t;
  using pointer                = T*;
  using const_pointer          = T const*;
  using reference              = T&;
  using const_reference        = T const&;
  using iterator               = T*;
  using const_iterator         = T const*;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  //@}

  /**\name Constructors */
  //@{
  constexpr Span() noexcept = default;
  constexpr Span(pointer ptr, index_type count) noexcept
    : m_buffer(ptr), m_size(count)
    {
      assert(! (!ptr) xor (!count));
    }
  constexpr Span(pointer first, pointer last) noexcept
    : Span(first, last - first)
    {
      assert(! (!first) xor (!last));
      assert(first <= last);
    }
  template <std::size_t N> constexpr Span(element_type (&arr)[N]) noexcept
    : Span(arr, N)
    {}

  /** Converting constructor from a contiguous container.
   * \pre The Container shall be contiguous
   * \warning The lifetime of the span shall not exceed the one of the container.
   * Be sure to not store the span locally, and initialize it from a rvalue.
   * The use case where a span is initialized from a rvalue shall be restricted
   * to function parameters.
   * \code
   * std::vector<T> f();
   * void g(Span<T> sp);
   * ...
   * Span<T> sp(f()); // NO!!! Use after release
   * g(f());          // OK!!
   * \endcode
   *
   * \todo static_assert the container is contiguous
   */
  template <class Container> constexpr Span(Container&& cont) noexcept
    : Span(cont.data(), cont.size())
    {
      // We cannot use op[] which has an assertion sometimes.
      // assert(&const[size()] == (&cont[0] + size()));
      // Beside, it's not noexcept.
    }
  template <class U> constexpr Span(const otb::Span<U>& s) noexcept
    : Span(s.data(), s.size())
    {}
  constexpr Span(const Span& other) noexcept = default;
  //@}

  /// shallow assignment
  Span& operator=(Span const&) noexcept = default;

  /// No-op destructor
  ~Span() = default;

  /**\name Iterators */
  //@{
  constexpr iterator       begin ()       noexcept { return data(); }
  constexpr iterator       end   ()       noexcept { return data()+size(); }
  constexpr const_iterator begin () const noexcept { return data(); }
  constexpr const_iterator end   () const noexcept { return data()+size(); }
  constexpr const_iterator cbegin() const noexcept { return data(); }
  constexpr const_iterator cend  () const noexcept { return data()+size(); }

  constexpr reverse_iterator       rbegin ()       noexcept { return reverse_iterator(end()); }
  constexpr reverse_iterator       rend   ()       noexcept { return reverse_iterator(begin()); }
  constexpr const_reverse_iterator crbegin() const noexcept { return reverse_const_iterator(cend()); }
  constexpr const_reverse_iterator crend  () const noexcept { return reverse_const_iterator(cbegin()); }
  //@}

  /**\name Element access */
  //@{
  constexpr pointer         data ()       noexcept { return m_buffer; }
  constexpr const_pointer   data () const noexcept { return m_buffer; }
  constexpr reference       front()       noexcept { assert(!empty()); return *data(); }
  constexpr const_reference front() const noexcept { assert(!empty()); return *data(); }
  constexpr reference       back ()       noexcept { assert(!empty()); return *data()+size()-1; }
  constexpr const_reference back () const noexcept { assert(!empty()); return *data()+size()-1; }

  constexpr reference       operator[](index_type p)       noexcept
  {
    assert(p < size());
    return data()[p];
  }
  constexpr const_reference operator[](index_type p) const noexcept
  {
    assert(p < size());
    return data()[p];
  }
  //@}

  /**\name Observers */
  //@{
  constexpr index_type size () const noexcept { return m_size; }
  constexpr bool       empty() const noexcept { return size() == 0; }
  //@}

  /**\name Subviews */
  //@{
  constexpr Span first(index_type n) const noexcept
  { assert(n < size()); return Span(data(), n);}
  constexpr Span last(index_type n) const noexcept
  { assert(n < size()); return Span(data()-n, n);}

  constexpr Span subspan(index_type offset, index_type count = std::numeric_limits<index_type>::max()) noexcept
  {
    assert(offset <= size());
    if (count == std::numeric_limits<index_type>::max())
    {
      count = size() - offset;
    }

    assert(count <= (size() - offset));
    return Span(data()+offset, count);

  }
  //@}

private:
  pointer    m_buffer = nullptr;
  index_type m_size   = 0;
};

/**
 * Helper function to make a span from a range defined with pointers.
 * Compensate the fact we are not have access to C++17 `span{ptr1, ptr2}`.
 * \tparam T  Auto deduced type of the elements
 * \param[in] first  start of the memory zone
 * \param[in] last   end of the memory zone
 * \return a span over `[first, last)`
 * \throw None
 * \pre `NOT first != nullptr XOR last != nullptr`
 * \pre `first < last`
 * \pre `[first, last)` can be iterated
 * \see `otb::Span<>`
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 */
template <typename T>
inline
auto make_span(T* first, T* last) noexcept
{
  return Span<T>(first, last);
}

/**
 * Helper function to make a span from a range defined with a pointer plus a
 * size.
 * Compensate the fact we are not have access to C++17 `span{ptr, count}`.
 * \tparam T  Auto deduced type of the elements
 * \param[in] first  start of the memory zone
 * \param[in] count  number of elements in the span.
 * \return a span over `[first, first+count)`
 * \throw None
 * \pre `NOT first != nullptr XOR count != 0`
 * \see `otb::Span<>`
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 */
template <typename T>
inline
auto make_span(T* first, std::size_t count) noexcept
{
  return Span<T>(first, count);
}

/**
 * Helper function to make a span from a static array.
 * Compensate the fact we are not have access to C++17 `span{array}`.
 * \tparam T  Auto deduced type of the elements
 * \tparam N  Auto deduced number of elements in the array
 * \param[in] array  static array
 * \return a span over `[&array[0], &array[N])`
 * \throw None
 * \see `otb::Span<>`
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 */
template <typename T, std::size_t N>
inline
auto make_span(T (&arr)[N]) noexcept
{
  return Span<T>(arr);
}

/**
 * Helper function to make a span from a contiguous container.
 * Compensate the fact we are not have access to C++17 `span{container}`.
 * \tparam ContiguousContainer  Auto deduced type of the container
 * \param[in] cont  container of contiguous elements
 * \return a span over `[c.data(), c.size())`
 * \throw None
 * \see `otb::Span<>`
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 */
template <typename ContiguousContainer>
inline
auto make_span(ContiguousContainer & c) noexcept
{
  return Span<decltype(*c.data())>(c);
}

}
// otb namespace


#endif // Span_h
