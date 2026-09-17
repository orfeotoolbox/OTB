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

/**@file otbITKIteratorHelpers.h
 * Define some helper functions to manipulate ITK iterators as C++ standard
 * iterators.
 *
 * @code
 * using InputIterator = itk::ImageScanlineConstIterator< ImageType >;
 * InputIterator  inIt(this->GetInput(), inputRegionForThread);
 * using OutputIterator = itk::ImageScanlineIterator< ImageType >;
 * OutputIterator outIt(this->GetOutput(), outputRegionForThread);
 *
 * for( ; !outIt.IsAtEnd() ; outIt+=itk::Line{}, inIt+=itk::Line{})
 * {
 *   for ( ; !inIt.IsAtEndOfLine() && !outIt.IsAtEndOfLine() ; ++outIt, ++inIt)
 *   {
 *     *outIt = *inIt;
 *   }
 * }
 * @endcode
 */

#ifndef otbITKIteratorHelpers_h
#define otbITKIteratorHelpers_h

#include <type_traits>
#include "otbMetaProgrammingLibrary.h"

namespace itk
{

// ========================================[ ++iterator
/**
 * Traits that tells whether it's an `Iterator` that has the `Next()` function.
 */
template <class, class = otb::void_t<>>
struct has_next : std::false_type {};

template <class IT>
struct has_next<IT, otb::void_t<decltype(std::declval<IT&>().Next())>>
: std::true_type {};

/**
 * Convenience constant that tells whether it's an `Iterator` that has the `Next()` function.
 */
template <class IT>
constexpr bool has_next_v = has_next<IT>::value;


template <typename Iterator>
inline
typename std::enable_if<has_next_v<Iterator>, Iterator>::type
operator++(Iterator& it, int)
{
  it.Next();
  return it;
}

template <typename Iterator>
inline
typename std::enable_if<! has_next_v<Iterator>, Iterator>::type
operator++(Iterator& , int)
{
  static_assert(has_next<Iterator>::value, "Not compatible with ITK iterators");
}


// ========================================[ iterator += Line{}
/**
 * Traits that tells whether it's a `ScanLineIterator` that has the `NextLine()` function.
 */
template <class, class = otb::void_t<>>
struct has_next_line : std::false_type {};

template <class IT>
struct has_next_line<IT, otb::void_t<decltype(std::declval<IT&>().NextLine())>>
: std::true_type {};

/**
 * Convenience constant that tells whether it's a `ScanLineIterator` that has the `NextLine()` function.
 */
template <class IT>
constexpr bool has_next_line_v = has_next_line<IT>::value;

/** Convenience to increment iterator to the next line.
 * Example:
 *
 * @code
 * using OutputIterator = itk::ImageScanlineIterator< ImageType >;
 * OutputIterator outIt(this->GetOutput(), outputRegionForThread);
 *
 * for( ; !outIt.IsAtEnd() ; outIt+=itk::Line{}, inIt+=itk::Line{})
 * {
 *   ...
 * }
 * @endcode
 */
struct Line {};


template <typename Iterator>
inline
typename std::enable_if<has_next_line_v<Iterator>, Iterator>::type &
operator+=(Iterator& it, Line)
{
  it.NextLine();
  return it;
}

template <typename Iterator>
inline
typename std::enable_if<! has_next_line_v<Iterator>, Iterator>::type
operator+=(Iterator&, Line)
{
  static_assert(has_next_line_v<Iterator>, "Not compatible with ITK scanline iterators");
}


// ========================================[ *iterator
template <typename TIterator>
struct ProxyDerefIterator
{
  using IteratorType = TIterator;
  using PixelType    = typename IteratorType::PixelType;

  ProxyDerefIterator(TIterator & it) : m_it(it) {}
  ProxyDerefIterator & operator=(PixelType const& v) {
    m_it.Set(v);
    return *this;
  }
  /*explicit*/ operator PixelType() const {
    return m_it.Get();
  }

#if 1
  decltype(auto) operator[](unsigned i) const {
    return m_it.Get()[i];
  }
#endif
private:
  IteratorType & m_it;
};

/**
 * Traits that tells whether it looks like an iterator that has the `Get()` function.
 */
template <class, class = otb::void_t<>>
struct has_get : std::false_type {};

template <class IT>
struct has_get<IT, otb::void_t<decltype(std::declval<IT&>().Get())>>
: std::true_type {};

/**
 * Convenience constant that tells whether it looks like an iterator that has the `Get()` function.
 */
template <class IT>
constexpr bool has_get_v = has_get<IT>::value;

/**
 * Traits that tells whether it looks like an iterator that has the `Set()` function.
 */
template <class, class = otb::void_t<>>
struct has_set : std::false_type {};

template <class IT>
struct has_set<IT,
  otb::void_t<decltype(
      std::declval<IT&>().Set(std::declval<typename IT::PixelType>()))>>
: std::true_type {};

/**
 * Convenience constant that tells whether it looks like an iterator that has the `Set()` function.
 */
template <class IT>
constexpr bool has_set_v = has_set<IT>::value;


/**
 * Inject dereference operator `*` into Read-Only ITK iterators.
 * \tparam Iterator  Any Read-Only ITK iterator
 * \param[in] it     the iterator instance
 *
 * \return a Read-Only proxy to pixel behind the iterator
 * \note The injection is done thanks to ADL
 */
template <typename Iterator>
inline
typename std::enable_if<
  has_get_v<Iterator> && !has_set_v<Iterator>,
  ProxyDerefIterator<Iterator const>>::type
operator*(Iterator const& it)
{
  ProxyDerefIterator<Iterator const> p{it};
  return p;
}

/**
 * Inject dereference operator `*` into Read-Write ITK iterators.
 * \tparam Iterator  Any Read-Write ITK iterator
 * \param[in] it     the iterator instance
 *
 * \return a Read-Write proxy to pixel behind the iterator
 * \note The injection is done thanks to ADL
 */
template <typename Iterator>
inline
typename std::enable_if<
  has_get_v<Iterator> && has_set_v<Iterator>,
  ProxyDerefIterator<Iterator>>::type
operator*(Iterator & it)
{
  ProxyDerefIterator<Iterator> p{it};
  return p;
}

#if 0
template <typename Iterator>
inline
typename std::enable_if<
!has_get<Iterator>::value && !has_set<Iterator>::value>::type
operator*(Iterator &)
{
  static_assert(has_get<Iterator>::value, "Not an input iterator");
  static_assert(has_set<Iterator>::value, "Not an output iterator");
}
#endif

}  // itk namespace

#endif  // otbITKIteratorHelpers_h
