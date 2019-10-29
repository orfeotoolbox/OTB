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


#ifndef ossimKeyWordListUtilities_h
#define ossimKeyWordListUtilities_h

#include "ossim/ossimStringUtilities.h"
#include "ossim/ossimXmlTools.h"
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimXmlNode.h>
#include <stdexcept>
#include <string>

namespace ossimplugins {
   class kw_runtime_error : public std::runtime_error {
   public:
      explicit kw_runtime_error(std::string const& msg)
         : std::runtime_error(msg)
         {}
   };
   class empty_keyword_data : public kw_runtime_error {
   public:
      explicit empty_keyword_data(std::string const& keyword)
         : kw_runtime_error("Data associated to '"+keyword+"' keyword is empty.")
         {}
   };

   class missing_keyword : public kw_runtime_error {
   public:
      explicit missing_keyword(std::string const& keyword)
         : kw_runtime_error("Cannot find '"+keyword+"' keyword in keyword list.")
         {}
   };

   inline
      void addOptional(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key,
            ossimXmlNode const& parentNode, ossimString const& xpath)
      {
         ossimRefPtr<ossimXmlNode> const& wh = parentNode.findFirstNode(xpath);
         if (wh.valid()) {
            ossimString const& value = wh->getText();
            kwl.addPair(prefix, key, value, true);
         }
      }

   inline
      void addOptional(ossimKeywordlist & kwl, std::string const& key,
            ossimXmlNode const& parentNode, ossimString const& xpath)
      {
         ossimRefPtr<ossimXmlNode> const& wh = parentNode.findFirstNode(xpath);
         if (wh.valid()) {
            ossimString const& value = wh->getText();
            kwl.addPair(key, value, true);
         }
      }

   inline
      ossimString const& addMandatory(ossimKeywordlist & kwl, std::string const& prefix, char const* key,
            ossimXmlNode const& parentNode, ossimString const& xpath)
      {
         ossimXmlNode const& childNode = getExpectedFirstNode(parentNode, xpath);
         ossimString const& value = childNode.getText();
         kwl.addPair(prefix, key, value, true);
         return value;
      }

   inline
      ossimString const& addMandatory(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key,
            ossimXmlNode const& parentNode, ossimString const& xpath)
      {
         ossimXmlNode const& childNode = getExpectedFirstNode(parentNode, xpath);
         ossimString const& value = childNode.getText();
         kwl.addPair(prefix, key, value, true);
         return value;
      }

   inline
      ossimString const& addMandatory(ossimKeywordlist & kwl, std::string const& prefix, ossimString const& key,
            ossimXmlNode const& parentNode, ossimString const& xpath)
      {
         ossimXmlNode const& childNode = getExpectedFirstNode(parentNode, xpath);
         ossimString const& value = childNode.getText();
         kwl.addPair(prefix, key.string(), value, true);
         return value;
      }

   inline
      ossimString const& addMandatory(ossimKeywordlist & kwl, std::string const& key,
            ossimXmlNode const& parentNode, ossimString const& xpath)
      {
         ossimXmlNode const& childNode = getExpectedFirstNode(parentNode, xpath);
         ossimString const& value = childNode.getText();
         kwl.addPair(key, value, true);
         return value;
      }

   struct ShallOverwrite {
      enum type { no, yes};
      ShallOverwrite(type o) : m_o(o) {}
      explicit ShallOverwrite(bool o) : m_o(o ? yes : no) {}
      bool shall_we() const { return m_o == yes; }
   private:
      type m_o;
   };

   inline
      std::size_t add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, std::size_t v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         const ossimString s = ossimString::toString(static_cast<ossim_uint64>(v));
         kwl.addPair(prefix, key, s.string(), overwrite.shall_we());
         return v;
      }
   inline
      std::size_t add(ossimKeywordlist & kwl, std::string const& key, std::size_t v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         const ossimString s = ossimString::toString(static_cast<ossim_uint64>(v));
         kwl.addPair(key, s.string(), overwrite.shall_we());
         return v;
      }
   inline
      std::string const& add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, std::string const& v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         kwl.addPair(prefix, key, v, overwrite.shall_we());
         return v;
      }
   inline
      std::string const& add(ossimKeywordlist & kwl, std::string const& key, std::string const& v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         kwl.addPair(key, v, overwrite.shall_we());
         return v;
      }
   template <typename DataType> inline
      DataType const& add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, DataType const& v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         const ossimString s = ossimString::toString(v);
         kwl.addPair(prefix, key, s.string(), overwrite.shall_we());
         return v;
      }

   template <typename DataType> inline
      DataType const& add(ossimKeywordlist & kwl, std::string const& key, DataType const& v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         const ossimString s = ossimString::toString(v);
         kwl.addPair(key, s.string(), overwrite.shall_we());
         return v;
      }

   inline
      ossimString const& add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, ossimString const& v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         kwl.addPair(prefix, key, v.string(), overwrite.shall_we());
         return v;
      }
   inline
      ossimString const& add(ossimKeywordlist & kwl, std::string const& prefix, ossimString const& v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         kwl.addPair(prefix, v, overwrite.shall_we());
         return v;
      }
   inline
      time::ModifiedJulianDate const& add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, time::ModifiedJulianDate const& v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         add(kwl, prefix, key, to_simple_string(v), overwrite);
         return v;
      }
   inline
      time::ModifiedJulianDate const& add(ossimKeywordlist & kwl, std::string const& key, time::ModifiedJulianDate const& v, ShallOverwrite overwrite = ShallOverwrite::yes)
      {
         add(kwl, key, to_simple_string(v), overwrite);
         return v;
      }
#if defined(USE_BOOST_TIME)
   inline
      boost::posix_time::ptime const& add(ossimKeywordlist & kwl, std::string const& key, boost::posix_time::ptime const& v)
      {
         add(kwl, key, to_iso_extended_string(v));
         return v;
      }
   inline
      boost::posix_time::ptime const& add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, boost::posix_time::ptime const& v)
      {
         return add(kwl, prefix+key, v);
      }

   inline void get(ossimKeywordlist const& kwl, std::string const& key, boost::posix_time::ptime & v)
   {
      std::string str = kwl.findKey(key);
      assert(&str != &ossimKeywordlist::NULL_KW);
      assert(!str.empty());
      std::replace(str.begin(), str.end(), 'T', ' ');
      v = boost::posix_time::time_from_string(str);
   }
   inline void get(ossimKeywordlist const& kwl, std::string const& prefix, std::string const& key, boost::posix_time::ptime & v)
   {
      get(kwl, prefix+key, v);
   }
#endif

   template <typename T>
   inline void get(ossimKeywordlist const& kwl, std::string const& key, T & v)
   {
      std::string const& str = kwl.findKey(key);
      if (&str == &ossimKeywordlist::NULL_KW) {
         throw missing_keyword(key);
      }
      if (str.empty()) {
         throw empty_keyword_data(key);
      }
      assert(&str != &ossimKeywordlist::NULL_KW);
      assert(!str.empty());
      v = to<T>(str, " extracting " + key + "from KWL");
   }

   template <typename T>
   inline void get(ossimKeywordlist const& kwl, std::string const& prefix, std::string const& key, T & v)
   {
      return get(kwl, prefix+key, v);
   }

   inline void get(ossimKeywordlist const& kwl, std::string const& key, time::ModifiedJulianDate & v)
   {
      std::string const& str = kwl.findKey(key);
      assert(&str != &ossimKeywordlist::NULL_KW);
      assert(!str.empty());
      v = time::toModifiedJulianDate(str);
   }

   inline void get(ossimKeywordlist const& kwl, std::string const& prefix, std::string const& key, time::ModifiedJulianDate & v)
   {
      get(kwl, prefix + key, v);
   }

} // ossimplugins namespace

#endif // ossimKeyWordListUtilities_h
