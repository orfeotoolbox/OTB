/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbOperatorUtilities_h
#define otbOperatorUtilities_h

#include <iostream>

namespace boostAdapter {
   // Uses Barton-Nackman trick to help implement operator on classes
   // Strongly inspired by boost.operators interface

   //TODO: imported from ossim plugins, but can we use boost instead ?

#define DEFINE_OPERATORS(name, compound, op)                                       \
   template <typename T> struct name ## 1                                          \
   {                                                                               \
      friend T operator op(T lhs, T const& rhs) {                                  \
         lhs compound rhs;                                                         \
         return lhs;                                                               \
      }                                                                            \
   };                                                                              \
   template <typename T, typename U> struct name ## 2                              \
   {                                                                               \
      friend T operator op(T lhs, U const& rhs) {                                  \
         lhs compound rhs;                                                         \
         return lhs;                                                               \
      }                                                                            \
      friend T operator op(U const& lhs, T rhs) {                                  \
         rhs compound lhs;                                                         \
         return rhs;                                                               \
      }                                                                            \
   };                                                                              \
   template <typename T, typename U=void> struct name         : name ## 2<T,U> {}; \
   template <typename T>                  struct name<T,void> : name ## 1<T>   {};

   template <typename U, typename V> struct substractable_asym
   {
      friend U operator-(V const& lhs, V const& rhs) {
         return V::template diff<U,V>(lhs, rhs);
      }
   };

   DEFINE_OPERATORS(addable,       +=, +);
   DEFINE_OPERATORS(substractable, -=, -);
   DEFINE_OPERATORS(multipliable,  *=, *);
#undef DEFINE_OPERATORS

   template <typename T, typename R> struct dividable {
      typedef R scalar_type;
      friend T operator/(T lhs, scalar_type const& rhs) {
         lhs /= rhs;
         return lhs;
      }
      friend scalar_type operator/(T const& lhs, T const& rhs) {
         return ratio_(lhs, rhs);
      }
   };

   template <typename T> struct streamable {
      friend std::ostream & operator<<(std::ostream & os, const T & v)
      { return v.display(os); }
      friend std::istream & operator>>(std::istream & is, T & v)
      { return v.read(is); }
   };

   template <typename T> struct less_than_comparable {
      friend bool operator>(T const& lhs, T const& rhs) {
         return rhs < lhs;
      }
      friend bool operator>=(T const& lhs, T const& rhs) {
         return !(lhs < rhs);
      }
      friend bool operator<=(T const& lhs, T const& rhs) {
         return !(rhs < lhs);
      }
   };

   template <typename T> struct equality_comparable {
      friend bool operator!=(T const& lhs, T const& rhs) {
         return !(rhs == lhs);
      }
   };

}// namespace boostAdapter

#endif // otbOperatorUtilities_h
