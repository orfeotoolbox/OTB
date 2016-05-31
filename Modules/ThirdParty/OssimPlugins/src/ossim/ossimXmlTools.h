//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
#include <boost/date_time/posix_time/posix_time.hpp>
#ifndef OSSIM_XMLTOOLS_H__
#define OSSIM_XMLTOOLS_H__

class ossimString;
// class ossimXmlNode;
#include "ossimStringUtilities.h"
#include "ossimTimeUtilities.h"
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimXmlNode.h>

namespace ossimplugins {
    typedef boost::posix_time::ptime         TimeType;

    ossimXmlNode const& getExpectedFirstNode(ossimXmlNode const& node, ossimString const& xpath);
    ossimString const& getTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
    ossimString const& getOptionalTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
    TimeType getTimeFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
    time::ModifiedJulianDate getModifiedJulianDateFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
    double getDoubleFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);

    template <typename DataType> inline
    DataType getFromFirstNode(ossimXmlNode const& node, ossimString const& xpath) {
       ossimXmlNode const& childNode = getExpectedFirstNode(node, xpath);
       ossimString const& sValue = childNode.getText();
       DataType const& value = to<DataType>(sValue);
       return value;
    }

    inline
    void addOptional(ossimKeywordlist & kwl, char const* prefix, char const* key,
          ossimXmlNode const& parentNode, ossimString const& xpath)
    {
       ossimRefPtr<ossimXmlNode> const& wh = parentNode.findFirstNode(xpath);
       if (wh.valid()) {
          ossimString const& value = wh->getText();
          kwl.add(prefix, key, value, true);
       }
    }

    inline
    void addOptional(ossimKeywordlist & kwl, char const* key,
          ossimXmlNode const& parentNode, ossimString const& xpath)
    {
       ossimRefPtr<ossimXmlNode> const& wh = parentNode.findFirstNode(xpath);
       if (wh.valid()) {
          ossimString const& value = wh->getText();
          kwl.add(key, value, true);
       }
    }

    inline
    ossimString const& addMandatory(ossimKeywordlist & kwl, char const* prefix, char const* key,
          ossimXmlNode const& parentNode, ossimString const& xpath)
    {
       ossimXmlNode const& childNode = getExpectedFirstNode(parentNode, xpath);
       ossimString const& value = childNode.getText();
       kwl.add(prefix, key, value, true);
       return value;
    }

    inline
    ossimString const& addMandatory(ossimKeywordlist & kwl, char const* prefix, ossimString const& key,
          ossimXmlNode const& parentNode, ossimString const& xpath)
    {
       ossimXmlNode const& childNode = getExpectedFirstNode(parentNode, xpath);
       ossimString const& value = childNode.getText();
       kwl.add(prefix, key.c_str(), value, true);
       return value;
    }

    inline
    ossimString const& addMandatory(ossimKeywordlist & kwl, char const* key,
          ossimXmlNode const& parentNode, ossimString const& xpath)
    {
       ossimXmlNode const& childNode = getExpectedFirstNode(parentNode, xpath);
       ossimString const& value = childNode.getText();
       kwl.add(key, value, true);
       return value;
    }

    template <typename DataType> inline
    void add(ossimKeywordlist & kwl, char const* prefix, char const * key, DataType const& v)
    {
       ossimString s = ossimString::toString(v);
       kwl.add(prefix, key, v);
    }
    inline
    void add(ossimKeywordlist & kwl, char const* prefix, char const * key, ossimString const& v)
    {
       kwl.add(prefix, key, v);
    }
    inline
    void add(ossimKeywordlist & kwl, char const* prefix, ossimString const& v)
    {
       kwl.add(prefix, v);
    }
    inline
    void add(ossimKeywordlist & kwl, char const* prefix, char const * key, time::ModifiedJulianDate const& v)
    {
       add(kwl, prefix, key, v.as_seconds());
    }
    inline
    void add(ossimKeywordlist & kwl, char const* prefix, time::ModifiedJulianDate const& v)
    {
       add(kwl, prefix, v.as_seconds());
    }
} // ossimplugins namespace


#endif // OSSIM_XMLTOOLS_H__
