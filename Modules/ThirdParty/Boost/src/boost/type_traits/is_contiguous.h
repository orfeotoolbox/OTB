/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef is_contiguous_h
#define is_contiguous_h

/**\ingroup boost
 * \file    is_contiguous.h
 * \since   OTB v 3.14.0
 *
 * \note This type traits is likelly to appear in boost and in C++ TR2 at some
 * point in time. It is used in order to (micro-)optimize the interfacing of OTB
 * and OGR.
 * \note The filename however does not follow boost naming policy yet.
 *
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html about
 * \c contiguous_iterator_tag.
 *
 * \todo What licence to use ?
 */

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/detail/container_fwd.hpp>
 
// forward declarations

// Comment the forward declaration of std::array as it causes compilation errors
// on MSVC 10 when the header boost/lexical_cast.hpp is also included
//namespace std {
//template <typename T, std::size_t N>                                class array;
//} // std namespace

namespace boost {
template <typename T, std::size_t N>                                 class array;
template <typename T, std::size_t N, typename TCloneAllocator>       class ptr_array;
template <typename T, typename TCloneAllocator, typename TAllocator> class ptr_vector;
} // boost namespace

namespace boost {
/**\ingroup boost
 * \brief Type-traits: are the elements contiguous in the range.
 * \note This type traits is likelly to appear in boost and in C++ TR2 at some
 * point in time. It is used in order to (micro-)optimize the interfacing of OTB
 * and OGR.
 * \note As a consequence, it follows C++ standard and boost naming policy.
 *
 * \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html about
 * \c contiguous_iterator_tag.
 * \since OTB v 3.14.0
 * @{
 */
template <typename Range> struct is_contiguous
  { enum {value = is_array<Range>::value || is_pointer<Range>::value}; };

// is_contiguous specialisations
template <typename T, typename TAllocator>         struct is_contiguous<std::vector<T, TAllocator> >
  { enum {value = true}; };
template <typename T, std::size_t N>               struct is_contiguous<boost::array<T, N> >
  { enum {value = true}; };
template <typename T, std::size_t N, typename TCA> struct is_contiguous<boost::ptr_array<T, N, TCA> >
  { enum {value = true}; };
template <typename T, typename TCA, typename TA>   struct is_contiguous<boost::ptr_vector<T, TCA, TA> >
  { enum {value = true}; };
/** @} */
} // boost namespace

#endif // __is_contiguous_h
