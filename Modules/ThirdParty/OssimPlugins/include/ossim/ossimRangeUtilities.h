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
