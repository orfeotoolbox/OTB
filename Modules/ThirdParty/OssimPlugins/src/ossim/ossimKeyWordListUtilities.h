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

   template <typename DataType> inline
      void add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, DataType const& v)
      {
         const ossimString s = ossimString::toString(v);
         kwl.addPair(prefix, key, s.string());
      }
   inline
      void add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, ossimString const& v)
      {
         kwl.addPair(prefix, key, v.string());
      }
   inline
      void add(ossimKeywordlist & kwl, std::string const& prefix, ossimString const& v)
      {
         kwl.addPair(prefix, v);
      }
   inline
      void add(ossimKeywordlist & kwl, std::string const& prefix, std::string const& key, time::ModifiedJulianDate const& v)
      {
         add(kwl, prefix, key, v.as_day_frac());
      }
   inline
      void add(ossimKeywordlist & kwl, std::string const& key, time::ModifiedJulianDate const& v)
      {
         add(kwl, key, v.as_day_frac());
      }

   template <typename T>
   inline void get(ossimKeywordlist const& kwl, std::string const& prefix, std::string const& key, T & v)
   {
      std::string const& str = kwl.findKey(prefix, key);
      assert(!str.empty());
      v = to<T>(str);
   }

   template <typename T>
   inline void get(ossimKeywordlist const& kwl, std::string const& key, T & v)
   {
      std::string const& str = kwl.findKey(key);
      assert(!str.empty());
      v = to<T>(str);
   }

} // ossimplugins namespace

#endif // ossimKeyWordListUtilities_h
