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

#ifndef ossimRangeUtilities_h
#define ossimRangeUtilities_h

namespace ossimplugins {
   template <typename T, std::size_t N>
      inline
      std::size_t size(T (&)[N])
      { return N; }

   template <typename T>
      inline
      typename T::size_type size(T const& cont)
      { return cont.size(); }

   template <typename T, std::size_t N>
      inline
      T* begin(T (&array)[N])
      { return &array[0]; }

   template <typename T>
      inline
      typename T::iterator
      begin(T & cont)
      {return begin(cont);}

   template <typename T>
      inline
      typename T::const_iterator
      begin(T const& cont)
      {return begin(cont);}

   template <typename T, std::size_t N>
      inline
      std::reverse_iterator<T*> rbegin(T (&array)[N])
      { return std::reverse_iterator<T*>(&array[N]); }

   template <typename T>
      inline
      typename T::reverse_iterator
      rbegin(T & cont)
      {return rbegin(cont);}

   template <typename T>
      inline
      typename T::const_reverse_iterator
      rbegin(T const& cont)
      {return rbegin(cont);}


   template <typename T, std::size_t N>
      inline
      T* end(T (&array)[N])
      { return &array[N]; }

   template <typename T>
      inline
      typename T::iterator
      end(T & cont)
      {return end(cont);}

   template <typename T>
      inline
      typename T::const_iterator
      end(T const& cont)
      {return end(cont);}

   template <typename T, std::size_t N>
      inline
      std::reverse_iterator<T*> rend(T (&array)[N])
      { return std::reverse_iterator<T*>(&array[0]); }

   template <typename T>
      inline
      typename T::reverse_iterator
      rend(T & cont)
      {return rend(cont);}

   template <typename T>
      inline
      typename T::const_reverse_iterator
      rend(T const& cont)
      {return rend(cont);}

   namespace details
   {
    template<typename T, std::size_t N>
       char (&array_size_helper(T (&)[N]))[N];
#define array_size(array) (sizeof ::ossimplugins::details::array_size_helper(array))
   } // details namespace

} // ossimplugins namespace

#endif // ossimRangeUtilities_h
