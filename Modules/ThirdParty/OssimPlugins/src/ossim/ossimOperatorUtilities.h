//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

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

   DEFINE_OPERATORS(addable, +=, +);
   DEFINE_OPERATORS(substractable, -=, -);
   DEFINE_OPERATORS(multipliable, *=, *);
#undef DEFINE_OPERATORS

   template <typename T, typename R> struct dividable {
      typedef R scalar_type;
      friend T operator/(T lhs, scalar_type const& rhs) {
         lhs /= rhs;
         return lhs;
      }
      friend scalar_type operator/(T const& lhs, T const& rhs) {
         return ratio(lhs, rhs);
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
