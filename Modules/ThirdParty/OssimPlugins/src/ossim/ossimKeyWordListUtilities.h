//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------

#ifndef ossimKeyWordListUtilities_h
#define ossimKeyWordListUtilities_h

#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimXmlNode.h>
#include "ossimStringUtilities.h"
#include "ossimXmlTools.h"

namespace ossimplugins {
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
      boost::posix_time::ptime const& add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, boost::posix_time::ptime const& v)
      {
         add(kwl, prefix, key, ossimString(to_iso_string(v)));
         return v;
      }
   inline
      boost::posix_time::ptime const& add(ossimKeywordlist & kwl, std::string const& key, boost::posix_time::ptime const& v)
      {
         add(kwl, key, to_iso_string(v));
         return v;
      }
#endif

   template <typename T>
   inline void get(ossimKeywordlist const& kwl, std::string const& prefix, std::string const& key, T & v)
   {
      std::string const& str = kwl.findKey(prefix, key);
      assert(&str != &ossimKeywordlist::NULL_KW);
      assert(!str.empty());
      v = to<T>(str, " extracting " + prefix + key + "from KWL");
   }

   template <typename T>
   inline void get(ossimKeywordlist const& kwl, std::string const& key, T & v)
   {
      std::string const& str = kwl.findKey(key);
      assert(&str != &ossimKeywordlist::NULL_KW);
      assert(!str.empty());
      v = to<T>(str, " extracting " + key + "from KWL");
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
