/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef ossimOperatorUtilities_h
#define ossimOperatorUtilities_h

namespace ossimplugins {
   // Uses Barton-Nackman trick to help implement operator on classes
   // Strongly inspired by boost.operators interface

   // See ossimTimeUtilities.h and ossimTimeUtilitiesTest.cpp for example of
   // use

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

}// namespace ossimplugins

#endif // ossimOperatorUtilities_h
